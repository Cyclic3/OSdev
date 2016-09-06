#ifndef _KERNEL_DISK_H
#define _KERNEL_DISK_H
#define	SATA_SIG_ATA			0x00000101	// SATA drive
#define	SATA_SIG_ATAPI			0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB			0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM			0x96690101	// Port multiplier
#define AHCI_DEV_NULL 			0
#define AHCI_DEV_SATA 			1
#define AHCI_DEV_SEMB 			2
#define AHCI_DEV_PM 			3
#define AHCI_DEV_SATAPI			4
#define HBA_PORT_IPM_ACTIVE 		1
#define HBA_PORT_DET_PRESENT 		3
#define	AHCI_BASE			0x40000
#define HBA_PxCMD_CR 			(1 << 15)
#define HBA_PxCMD_FR			(1 << 14)
#define HBA_PxCMD_FRE			(1 <<  4)
#define HBA_PxCMD_SUD			(1 <<  1)
#define HBA_PxCMD_ST			(1 <<  0)
#define ATA_DEV_BUSY 			0x80
#define ATA_DEV_DRQ 			0x08
#define ATA_CMD_READ_DMA_EX		0x25
#define ATA_CMD_WRITE_DMA_EX	        0x35
#define HBA_PxIS_TFES 			(1 << 30)
#define HBA_MEM_PCI_OFFSET              0x24
#define ATA_CMD_READ_PIO                0x20
#define ATA_CMD_READ_PIO_EXT            0x24
#define ATA_CMD_READ_DMA                0xC8
#define ATA_CMD_READ_DMA_EXT            0x25
#define ATA_CMD_WRITE_PIO               0x30
#define ATA_CMD_WRITE_PIO_EXT           0x34
#define ATA_CMD_WRITE_DMA               0xCA
#define ATA_CMD_WRITE_DMA_EXT           0x35
#define ATA_CMD_CACHE_FLUSH             0xE7
#define ATA_CMD_CACHE_FLUSH_EXT         0xEA
#define ATA_CMD_PACKET                  0xA0
#define ATA_CMD_IDENTIFY_PACKET         0xA1
#define ATA_CMD_IDENTIFY                0xEC
#include <kernel/io.h>
typedef struct
{
      unsigned short   config;      /* lots of obsolete bit flags */
      unsigned short   cyls;      /* obsolete */
      unsigned short   reserved2;   /* special config */
      unsigned short   heads;      /* "physical" heads */
      unsigned short   track_bytes;   /* unformatted bytes per track */
      unsigned short   sector_bytes;   /* unformatted bytes per sector */
      unsigned short   sectors;   /* "physical" sectors per track */
      unsigned short   vendor0;   /* vendor unique */
      unsigned short   vendor1;   /* vendor unique */
      unsigned short   vendor2;   /* vendor unique */
      unsigned char   serial_no[20];   /* 0 = not_specified */
      unsigned short   buf_type;
      unsigned short   buf_size;   /* 512 byte increments; 0 = not_specified */
      unsigned short   ecc_bytes;   /* for r/w long cmds; 0 = not_specified */
      unsigned char   fw_rev[8];   /* 0 = not_specified */
      unsigned char   model[40];   /* 0 = not_specified */
      unsigned short  multi_count; /* Multiple Count */
      unsigned short   dword_io;   /* 0=not_implemented; 1=implemented */
      unsigned short   capability1;   /* vendor unique */
      unsigned short   capability2;   /* bits 0:DMA 1:LBA 2:IORDYsw 3:IORDYsup word: 50 */
      unsigned char   vendor5;   /* vendor unique */
      unsigned char   tPIO;      /* 0=slow, 1=medium, 2=fast */
      unsigned char   vendor6;   /* vendor unique */
      unsigned char   tDMA;      /* 0=slow, 1=medium, 2=fast */
      unsigned short   field_valid;   /* bits 0:cur_ok 1:eide_ok */
      unsigned short   cur_cyls;   /* logical cylinders */
      unsigned short   cur_heads;   /* logical heads word 55*/
      unsigned short   cur_sectors;   /* logical sectors per track */
      unsigned short   cur_capacity0;   /* logical total sectors on drive */
      unsigned short   cur_capacity1;   /*  (2 words, misaligned int)     */
      unsigned char   multsect;   /* current multiple sector count */
      unsigned char   multsect_valid;   /* when (bit0==1) multsect is ok */
      unsigned int   lba_capacity;   /* total number of sectors */
      unsigned short   dma_1word;   /* single-word dma info */
      unsigned short   dma_mword;   /* multiple-word dma info */
      unsigned short  eide_pio_modes; /* bits 0:mode3 1:mode4 */
      unsigned short  eide_dma_min;   /* min mword dma cycle time (ns) */
      unsigned short  eide_dma_time;   /* recommended mword dma cycle time (ns) */
      unsigned short  eide_pio;       /* min cycle time (ns), no IORDY  */
      unsigned short  eide_pio_iordy; /* min cycle time (ns), with IORDY */
      unsigned short   words69_70[2];   /* reserved words 69-70 */
      unsigned short   words71_74[4];   /* reserved words 71-74 */
      unsigned short  queue_depth;   /*  */
      unsigned short  sata_capability;   /*  SATA Capabilities word 76*/
      unsigned short  sata_additional;   /*  Additional Capabilities */
      unsigned short  sata_supported;   /* SATA Features supported  */
      unsigned short  features_enabled;   /* SATA features enabled */
      unsigned short  major_rev_num;   /*  Major rev number word 80 */
      unsigned short  minor_rev_num;   /*  */
      unsigned short  command_set_1;   /* bits 0:Smart 1:Security 2:Removable 3:PM */
      unsigned short  command_set_2;   /* bits 14:Smart Enabled 13:0 zero */
      unsigned short  cfsse;      /* command set-feature supported extensions */
      unsigned short  cfs_enable_1;   /* command set-feature enabled */
      unsigned short  cfs_enable_2;   /* command set-feature enabled */
      unsigned short  csf_default;   /* command set-feature default */
      unsigned short  dma_ultra;   /*  */
      unsigned short   word89;      /* reserved (word 89) */
      unsigned short   word90;      /* reserved (word 90) */
      unsigned short   CurAPMvalues;   /* current APM values */
      unsigned short   word92;         /* reserved (word 92) */
      unsigned short   comreset;      /* should be cleared to 0 */
      unsigned short  accoustic;      /*  accoustic management */
      unsigned short  min_req_sz;      /* Stream minimum required size */
      unsigned short  transfer_time_dma;   /* Streaming Transfer Time-DMA */
      unsigned short  access_latency;      /* Streaming access latency-DMA & PIO WORD 97*/
      unsigned int    perf_granularity;   /* Streaming performance granularity */
      unsigned char   total_usr_sectors[3]; /* Total number of user addressable sectors */
      unsigned short  word104;
      unsigned short   transfer_time_pio;    /* Streaming Transfer time PIO */
      unsigned short   reserved105;       /* Word 105 */
      unsigned short   sector_sz;          /* Puysical Sector size / Logical sector size */
      unsigned short   inter_seek_delay;   /* In microseconds */
      unsigned short   words108_116[9];    /*  */
      unsigned int   words_per_sector;    /* words per logical sectors */
      unsigned short   supported_settings; /* continued from words 82-84 */
      unsigned short   command_set_3;       /* continued from words 85-87 */
      unsigned short  words121_126[6];   /* reserved words 121-126 */
      unsigned short   word127;         /* reserved (word 127) */
      unsigned short   security_status;   /* device lock function
                   * 15:9   reserved
                   * 8   security level 1:max 0:high
                   * 7:6   reserved
                   * 5   enhanced erase
                   * 4   expire
                   * 3   frozen
                   * 2   locked
                   * 1   en/disabled
                   * 0   capability
                   */
      unsigned short  csfo;      /* current set features options
                   * 15:4   reserved
                   * 3   auto reassign
                   * 2   reverting
                   * 1   read-look-ahead
                   * 0   write cache
                   */
      unsigned short   words130_155[26];/* reserved vendor words 130-155 */
      unsigned short   word156;
      unsigned short   words157_159[3];/* reserved vendor words 157-159 */
      unsigned short   cfa; /* CFA Power mode 1 */
      unsigned short   words161_175[15]; /* Reserved */
      unsigned char   media_serial[60]; /* words 176-205 Current Media serial number */
      unsigned short   sct_cmd_transport; /* SCT Command Transport */
      unsigned short   words207_208[2]; /* reserved */
      unsigned short   block_align; /* Alignement of logical blocks in larger physical blocks */
      unsigned int   WRV_sec_count; /* Write-Read-Verify sector count mode 3 only */
      unsigned int      verf_sec_count; /* Verify Sector count mode 2 only */
      unsigned short   nv_cache_capability; /* NV Cache capabilities */
      unsigned short   nv_cache_sz; /* NV Cache size in logical blocks */
      unsigned short   nv_cache_sz2; /* NV Cache size in logical blocks */
      unsigned short   rotation_rate; /* Nominal media rotation rate */
      unsigned short   reserved218; /*  */
      unsigned short   nv_cache_options; /* NV Cache options */
      unsigned short   words220_221[2]; /* reserved */
      unsigned short   transport_major_rev; /*  */
      unsigned short   transport_minor_rev; /*  */
      unsigned short   words224_233[10]; /* Reserved */
      unsigned short   min_dwnload_blocks; /* Minimum number of 512byte units per DOWNLOAD MICROCODE
                                  command for mode 03h */
      unsigned short   max_dwnload_blocks; /* Maximum number of 512byte units per DOWNLOAD MICROCODE
                                  command for mode 03h */
      unsigned short   words236_254[19];   /* Reserved */
      unsigned short   integrity;          /* Cheksum, Signature */
} AHCI_IDENTIFY;
typedef enum
{
	FIS_TYPE_REG_H2D	= 0x27,	// Register FIS - host to device
	FIS_TYPE_REG_D2H	= 0x34,	// Register FIS - device to host
	FIS_TYPE_DMA_ACT	= 0x39,	// DMA activate FIS - device to host
	FIS_TYPE_DMA_SETUP	= 0x41,	// DMA setup FIS - bidirectional
	FIS_TYPE_DATA		= 0x46,	// Data FIS - bidirectional
	FIS_TYPE_BIST		= 0x58,	// BIST activate FIS - bidirectional
	FIS_TYPE_PIO_SETUP	= 0x5F,	// PIO setup FIS - device to host
	FIS_TYPE_DEV_BITS	= 0xA1,	// Set device bits FIS - device to host
} FIS_TYPE;
typedef struct __attribute__((packed))
{
	// DWORD 0
	unsigned char	fis_type;	// FIS_TYPE_REG_H2D
 
	unsigned char	pmport:4;	// Port multiplier
	unsigned char	rsv0:3;		// Reserved
	unsigned char	c:1;		// 1: Command, 0: Control
 
	unsigned char	command;	// Command register
	unsigned char	featurel;	// Feature register, 7:0
 
	// DWORD 1
	unsigned char	lba0;		// LBA low register, 7:0
	unsigned char	lba1;		// LBA mid register, 15:8
	unsigned char	lba2;		// LBA high register, 23:16
	unsigned char	device;		// Device register
 
	// DWORD 2
	unsigned char	lba3;		// LBA register, 31:24
	unsigned char	lba4;		// LBA register, 39:32
	unsigned char	lba5;		// LBA register, 47:40
	unsigned char	featureh;	// Feature register, 15:8
 
	// DWORD 3
	unsigned char	countl;		// Count register, 7:0
	unsigned char	counth;		// Count register, 15:8
	unsigned char	icc;		// Isochronous command completion
	unsigned char	control;	// Control register
 
	// DWORD 4
	unsigned char	rsv1[4];	// Reserved
} FIS_REG_H2D;
typedef struct __attribute__((packed))
{
	// DWORD 0
	unsigned char	fis_type;    // FIS_TYPE_REG_D2H
 
	unsigned char	pmport:4;    // Port multiplier
	unsigned char	rsv0:2;      // Reserved
	unsigned char	i:1;         // Interrupt bit
	unsigned char	rsv1:1;      // Reserved
 
	unsigned char	status;      // Status register
	unsigned char	error;       // Error register
 
	// DWORD 1
	unsigned char	lba0;        // LBA low register, 7:0
	unsigned char	lba1;        // LBA mid register, 15:8
	unsigned char	lba2;        // LBA high register, 23:16
	unsigned char	device;      // Device register
 
	// DWORD 2
	unsigned char	lba3;        // LBA register, 31:24
	unsigned char	lba4;        // LBA register, 39:32
	unsigned char	lba5;        // LBA register, 47:40
	unsigned char	rsv2;        // Reserved
 
	// DWORD 3
	unsigned char	countl;      // Count register, 7:0
	unsigned char	counth;      // Count register, 15:8
	unsigned char	rsv3[2];     // Reserved
 
	// DWORD 4
	unsigned char	rsv4[4];     // Reserved
} FIS_REG_D2H;
typedef struct __attribute__((packed))
{
	// DWORD 0
	unsigned char	fis_type;	// FIS_TYPE_DATA
 
	unsigned char	pmport:4;	// Port multiplier
	unsigned char	rsv0:4;		// Reserved
 
	unsigned char	rsv1[2];	// Reserved
 
	// DWORD 1 ~ N
	unsigned int	data[0];	// Payload
} FIS_DATA;
typedef struct __attribute__((packed))
{
	// DWORD 0
	unsigned char	fis_type;	// FIS_TYPE_PIO_SETUP
 
	unsigned char	pmport:4;	// Port multiplier
	unsigned char	rsv0:1;		// Reserved
	unsigned char	d:1;		// Data transfer direction, 1 - device to host
	unsigned char	i:1;		// Interrupt bit
	unsigned char	rsv1:1;
 
	unsigned char	status;		// Status register
	unsigned char	error;		// Error register
 
	// DWORD 1
	unsigned char	lba0;		// LBA low register, 7:0
	unsigned char	lba1;		// LBA mid register, 15:8
	unsigned char	lba2;		// LBA high register, 23:16
	unsigned char	device;		// Device register
 
	// DWORD 2
	unsigned char	lba3;		// LBA register, 31:24
	unsigned char	lba4;		// LBA register, 39:32
	unsigned char	lba5;		// LBA register, 47:40
	unsigned char	rsv2;		// Reserved
 
	// DWORD 3
	unsigned char	countl;		// Count register, 7:0
	unsigned char	counth;		// Count register, 15:8
	unsigned char	rsv3;		// Reserved
	unsigned char	e_status;	// New value of status register
 
	// DWORD 4
	unsigned short	tc;			// Transfer count
	unsigned char	rsv4[2];	// Reserved
} FIS_PIO_SETUP;
typedef struct __attribute__((packed))
{
	// DWORD 0
	unsigned char	fis_type;	// FIS_TYPE_DMA_SETUP
 
	unsigned char	pmport:4;	// Port multiplier
	unsigned char	rsv0:1;		// Reserved
	unsigned char	d:1;		// Data transfer direction, 1 - device to host
	unsigned char	i:1;		// Interrupt bit
	unsigned char	a:1;            // Auto-activate. Specifies if DMA Activate FIS is needed
 
    unsigned char    rsved[2];       // Reserved
 
	//DWORD 1&2
 
    unsigned long long DMAbufferID;    // DMA Buffer Identifier. Used to Identify DMA buffer in host memory. SATA Spec says host specific and not in Spec. Trying AHCI spec might work.
 
    //DWORD 3
    unsigned int rsvd;           //More reserved
 
    //DWORD 4
    unsigned int   DMAbufOffset;   //unsigned char offset into buffer. First 2 bits must be 0
 
    //DWORD 5
    unsigned int   TransferCount;  //Number of unsigned chars to transfer. Bit 0 must be 0
 
    //DWORD 6
    unsigned int   resvd;          //Reserved
 
} FIS_DMA_SETUP;
typedef volatile struct __attribute__((packed))
{
	unsigned int	clb;		// 0x00, command list base address, 1K-unsigned char aligned
	unsigned int	clbu;		// 0x04, command list base address upper 32 bits
	unsigned int	fb;		// 0x08, FIS base address, 256-unsigned char aligned
	unsigned int	fbu;		// 0x0C, FIS base address upper 32 bits
	unsigned int	is;		// 0x10, interrupt status
	unsigned int	ie;		// 0x14, interrupt enable
	unsigned int	cmd;		// 0x18, command and status
	unsigned int	rsv0;		// 0x1C, Reserved
	unsigned int	tfd;		// 0x20, task file data
	unsigned int	sig;		// 0x24, signature
	unsigned int	ssts;		// 0x28, SATA status (SCR0:SStatus)
	unsigned int	sctl;		// 0x2C, SATA control (SCR2:SControl)
	unsigned int	serr;		// 0x30, SATA error (SCR1:SError)
	unsigned int	sact;		// 0x34, SATA active (SCR3:SActive)
	unsigned int	ci;		// 0x38, command issue
	unsigned int	sntf;		// 0x3C, SATA notification (SCR4:SNotification)
	unsigned int	fbs;		// 0x40, FIS-based switch control
	unsigned int	rsv1[11];	// 0x44 ~ 0x6F, Reserved
	unsigned int	vendor[4];	// 0x70 ~ 0x7F, vendor specific
} HBA_PORT;
typedef volatile struct __attribute__((packed))
{
	// 0x00 - 0x2B, Generic Host Control
	unsigned int	cap;		// 0x00, Host capability
	unsigned int	ghc;		// 0x04, Global host control
	unsigned int	is;			// 0x08, Interrupt status
	unsigned int	pi;			// 0x0C, Port implemented
	unsigned int	vs;			// 0x10, Version
	unsigned int	ccc_ctl;	// 0x14, Command completion coalescing control
	unsigned int	ccc_pts;	// 0x18, Command completion coalescing ports
	unsigned int	em_loc;		// 0x1C, Enclosure management location
	unsigned int	em_ctl;		// 0x20, Enclosure management control
	unsigned int	cap2;		// 0x24, Host capabilities extended
	unsigned int	bohc;		// 0x28, BIOS/OS handoff control and status
 
	// 0x2C - 0x9F, Reserved
	unsigned char	rsv[0xA0-0x2C];
 
	// 0xA0 - 0xFF, Vendor specific registers
	unsigned char	vendor[0x100-0xA0];
 
	// 0x100 - 0x10FF, Port control registers
	HBA_PORT	ports[1];	// 1 ~ 32
} HBA_MEM;
typedef volatile struct __attribute__((packed))
{
	// 0x00
	unsigned char err;		// Error
	unsigned char r1:1;		// Reserved bit
	unsigned char status1:3;// Status High
	unsigned char r2:1; 	// Reserved bit
	unsigned char status2:3;// Status Low
	//0x01
	unsigned char r3:1; 	// Reserved bit
	unsigned char i:1; 		// No idea what this is
	unsigned char r4:1; 	// Reserved bit
	unsigned char r5:5; 	// Reserved bits
	//0x02
	unsigned char fis_type;	// FIS_TYPE_DEV_BITS
}FIS_DEV_BITS;
typedef volatile struct __attribute__((packed))
{
	// 0x00
	FIS_DMA_SETUP		dsfis;		// DMA Setup FIS
	unsigned char		pad0[4];
 
	// 0x20
	FIS_PIO_SETUP	psfis;		// PIO Setup FIS
	unsigned char		pad1[12];
 
	// 0x40
	FIS_REG_D2H	rfis;		// Register – Device to Host FIS
	unsigned char		pad2[4];
 
	// 0x58
	FIS_DEV_BITS	sdbfis;		// Set Device Bit FIS
 
	// 0x60
	unsigned char		ufis[64];
 
	// 0xA0
	unsigned char		rsv[0x100-0xA0];
} HBA_FIS;
typedef struct __attribute__((packed))
{
	// DW0
	unsigned char	cfl:5;		// Command FIS length in DWORDS, 2 ~ 16
	unsigned char	a:1;		// ATAPI
	unsigned char	w:1;		// Write, 1: H2D, 0: D2H
	unsigned char	p:1;		// Prefetchable
 
	unsigned char	r:1;		// Reset
	unsigned char	b:1;		// BIST
	unsigned char	c:1;		// Clear busy upon R_OK
	unsigned char	rsv0:1;		// Reserved
	unsigned char	pmp:4;		// Port multiplier port
 
	unsigned short	prdtl;		// Physical region descriptor table length in entries
 
	// DW1
	volatile
	unsigned int	prdbc;		// Physical region descriptor unsigned char count transferred
 
	// DW2, 3
	unsigned int	ctba;		// Command table descriptor base address
	unsigned int	ctbau;		// Command table descriptor base address upper 32 bits
 
	// DW4 - 7
	unsigned int	rsv1[4];	// Reserved
} HBA_CMD_HEADER;

typedef struct __attribute__((packed))
{
	unsigned int	dba;		// Data base address
	unsigned int	dbau;		// Data base address upper 32 bits
	unsigned int	rsv0;		// Reserved
 
	// DW3
	unsigned int	dbc:22;		// unsigned char count, 4M max
	unsigned int	rsv1:9;		// Reserved
	unsigned int	i:1;		// Interrupt on completion
} HBA_PRDT_ENTRY;

typedef struct __attribute__((packed))
{
	// 0x00
	unsigned char	cfis[64];	// Command FIS
 
	// 0x40
	unsigned char	acmd[16];	// ATAPI command, 12 or 16 unsigned chars
 
	// 0x50
	unsigned char	rsv[48];	// Reserved
 
	// 0x80
	HBA_PRDT_ENTRY	prdt_entry[0];	// Physical region descriptor table entries, 0 ~ 65535
} HBA_CMD_TBL;


void probe_port(int ports[32]);
unsigned char read(HBA_PORT *port, unsigned int startl, unsigned int starth, unsigned int count, unsigned short *buf);
unsigned char write(HBA_PORT *port, unsigned int startl, unsigned int starth, unsigned int count, unsigned short *buf);
HBA_MEM * PCI2HBA(struct pci_device dev);
HBA_MEM * abar;
AHCI_IDENTIFY * identify_disk(HBA_PORT *port, AHCI_IDENTIFY * id);
#endif
