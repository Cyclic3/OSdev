#include <kernel/disk.h>
#include <kernel/io.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <limits.h>
HBA_MEM * PCI2HBA(struct pci_device dev) {return (HBA_MEM *) pciConfigReadReg(dev.bus,dev.device,dev.function,HBA_MEM_PCI_OFFSET);}
unsigned char get_abar(){
    int j = n_devices-1;
    for (;j == -1;j--) if(devices[j].class==0x0601) break;
    if (j < 0) return -1;
    abar = PCI2HBA(devices[j]);
    return 0;
}
// Check device type
static int check_type(HBA_PORT *port)
{
	unsigned int ssts = port->ssts;
 
	unsigned char ipm = (ssts >> 8) & 0x0F;
	unsigned char det = ssts & 0x0F;
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}
// Start command engine
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	//while (port->cmd & HBA_PxCMD_CR);
 
	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST; 
}
 
// Stop command engine
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;
 
	// Wait until FR (bit14), CR (bit15) are cleared
/*
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
 */
	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;
}
void port_rebase(HBA_PORT *port, int portno)
{
	stop_cmd(port);	// Stop command engine
 
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE + (portno<<10);
	port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32<<10) + (portno<<8);
	port->fbu = 0;
	memset((void*)(port->fb), 0, 256);
 
	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}
 
	start_cmd(port);	// Start command engine
}

void probe_port(int ports[32])
{
	// Search disk in impelemented ports
	unsigned int pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
                        ports[i] = dt;
			if (dt == AHCI_DEV_SATA)
			{
				puts("SATA");
                                port_rebase(abar->ports,i);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				puts("SATAPI");
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				puts("SEMB");
			}
			else if (dt == AHCI_DEV_PM)
			{
				puts("PM");
			}
			else
			{
				//puts("Now");
			}
		}
 
		pi >>= 1;
		i ++;
	}
}
// Find a free command list slot
int find_cmdslot(HBA_PORT *port)
{
	// If not set in SACT and CI, the slot is free
	unsigned int slots = (port->sact | port->ci);
	for (int i=0; i<32; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
	puts("Cannot find free command list entry");
	return -1;
}
unsigned char read(HBA_PORT *port, unsigned int startl, unsigned int starth, unsigned int count, unsigned short *buf)
{
	port->is = (unsigned int)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return 0;
 
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(unsigned int);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (unsigned short)((count-1)>>4) + 1;	// PRDT entries count
 
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
 		(cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
        int i = 0;
	// 8K bytes (16 sectors) per PRDT
	for (; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (unsigned int)buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (unsigned int)buf;
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (unsigned char)startl;
	cmdfis->lba1 = (unsigned char)(startl>>8);
	cmdfis->lba2 = (unsigned char)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (unsigned char)(startl>>24);
	cmdfis->lba4 = (unsigned char)starth;
	cmdfis->lba5 = (unsigned char)(starth>>8);
 
	cmdfis->countl = (unsigned char)count;
	cmdfis->counth = (unsigned char)(count>>8);
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		puts("Port is hung");
		return -1;
	}
 
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & HBA_PxIS_TFES)	// Task file error
		{
			puts("TFE1");
			return -1;
		}
	}
 
	// Check again
	if (port->is & HBA_PxIS_TFES)
	{
		puts("TFE2");
		return -1;
	}
 
	return 0;
}
unsigned char write(HBA_PORT *port, unsigned int startl, unsigned int starth, unsigned int count, unsigned short *buf)
{
	port->is = (unsigned int)-1;		// Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = find_cmdslot(port);
	if (slot == -1)
		return -1;
 
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->cfl = sizeof(FIS_REG_H2D)/sizeof(unsigned int);	// Command FIS size
	cmdheader->w = 1;		// Write to device
	cmdheader->prdtl = (unsigned short)((count-1)>>4) + 1;	// PRDT entries count
 
	HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)(cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(HBA_CMD_TBL) +
 		(cmdheader->prdtl-1)*sizeof(HBA_PRDT_ENTRY));
        int i = 0;
	// 8K bytes (16 sectors) per PRDT
	for (; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (unsigned int)buf;
		cmdtbl->prdt_entry[i].dbc = 8*1024;	// 8K bytes
		cmdtbl->prdt_entry[i].i = 1;
		buf += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (unsigned int)buf;
	cmdtbl->prdt_entry[i].dbc = count<<9;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	FIS_REG_H2D *cmdfis = (FIS_REG_H2D*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_WRITE_DMA_EX;
 
	cmdfis->lba0 = (unsigned char)startl;
	cmdfis->lba1 = (unsigned char)(startl>>8);
	cmdfis->lba2 = (unsigned char)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (unsigned char)(startl>>24);
	cmdfis->lba4 = (unsigned char)starth;
	cmdfis->lba5 = (unsigned char)(starth>>8);
 
	cmdfis->countl = (unsigned char)count;
	cmdfis->counth = (unsigned char)(count>>8);
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
	}
	if (spin == 1000000)
	{
		puts("Port is hung");
		return -1;
	}
 
	port->ci = 1;//<<slot;	// Issue command
        while (port->ci != 0);
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & HBA_PxIS_TFES)	// Task file error
		{
			puts("TFE1");
			return -1;
		}
	}
 
	// Check again
	if (port->is & HBA_PxIS_TFES)
	{
		puts("TFE2");
		return -1;
	}
 
	return 0;
}
