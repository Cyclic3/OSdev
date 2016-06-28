#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

char* hexdump(unsigned int v, char vvv[9]) 
{
	const char vv[] = "0123456789abcdef";
	vvv[0] = vv[(v/268435456)];
	v=(v%268435456);
	vvv[1] = vv[(v/16777216)];
	v=(v%16777216);
	vvv[2] = vv[(v/1048576)];
	v=(v%1048576);
	vvv[3] = vv[(v/65536)];
	v=(v%65536);
	vvv[4] = vv[(v/4096)];
	v=(v%4096);
	vvv[5] = vv[(v/256)];
	v=(v%256);
	vvv[6] = vv[v/16];
	v=(v%16);
	vvv[7] = vv[v];
	vvv[8] = '\0';return vvv;
}

#include <kernel/tty.h>

#include <kernel/vga.h>

#include <kernel/io.h>

#include <kernel/disk.h>

#include <kernel/cpu.h>

void halt(){asm volatile ("hlt");}
void kernel_early(void)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, 0xFFFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((0xFFFF>>8)&0xFF));
    terminal_initialise();
}

char* pad_num(char* v){
	if(strlen(v)==1) {char* dest = "00"; memcpy(dest+1,v,1); return dest;} else {return v;}
	
}
struct div_t{
  int quot;
  int rem;
};
struct div_t div (int a, int b)
{
    struct div_t c = {.quot = a/b,.rem = a%b};
    return c;
}
struct time{
    char hours;
    char minutes;
    char seconds;
};
struct time get_time(){
    struct time t;
    char s = ask_cmos(0x00);
	char m = ask_cmos(0x02);
	char h = ask_cmos(0x04);
    s = (s & 0x0F)+(s/16)*10;
    m = (m & 0x0F) + ((m / 16) * 10);
    h = ((h & 0x0F) + (((h & 0x70) / 16) * 10) ) | (h & 0x80);
    t.hours = h;
    t.minutes = m;
    t.seconds = s;
    return t;
}
struct time add_time(struct time a, struct time b){
    struct time c;
    struct div_t ss = div(a.seconds+b.seconds,60);
    c.seconds = ss.rem;
    struct div_t mm = div(a.minutes+b.minutes,60);

    c.minutes = mm.rem+ss.quot;
    struct div_t hh = div(a.hours+b.hours,24);
    c.hours = hh.rem + mm.quot;
    
    return c;
}
void print_time(struct time now)
{
    char* hh;
	printf(pad_num(itoa(now.hours,hh,10)));
	printf(":");
	char* mm;
	printf(pad_num(itoa(now.minutes,mm,10)));
	printf(":");
	char* ss;
	printf(pad_num(itoa(now.seconds,ss,10)));
}
char equal_time(struct time a, struct time b)
{
    return (a.seconds==b.seconds)&(a.minutes==b.minutes)&(a.hours==b.hours);
}
int atoi(char *p)
{
    int k = 0;
    while (*p) {
        k = (k<<3)+(k<<1)+(*p)-'0';
        p++;
     }
    return k;
}
void dump(unsigned int * p)
{
    char vvv[9] = "000000000";
    unsigned int lines = 0;
    while(1)
    {
    	//if (lines == 25){terminal_initialise();lines = 0;}
    	readcode();
    	unsigned int * j = p;
    	printf(hexdump((p),vvv));
    	printf("\t");
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf("\t\t");
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	puts("");
    	lines++;
    }
}	/*
	puts(itoa(readchar(),v,10));
	printf ("TTL: ");
	char* tt = readline(v,1);
	int t = atoi(tt);
	puts(tt);
	puts(itoa(t,v,10));
	struct time init = get_time();
	struct time add = {.seconds=t};
	struct time stop = add_time(init,add);
	printf("Halting at: ");
	print_time(stop);
	printf("\n");
	char s = ask_cmos(0x00);
	while(1){
		//terminal_initialize();        
        s = ask_cmos(0x00);
		clearline();
        struct time now = get_time();
        print_time(now);
        if (equal_time(now,stop)) break;
		while(ask_cmos(0x00)==s);
	}
	terminal_initialise();
	puts("GOING DOWN NOW!");
	puts("But first, because it looks cool, we shall dump memory!");
	char vvv[9] = "000000000";
	unsigned char * p = 0x10000000;
	puts(p);
	puts(hexdump(&p,vvv));
	puts(hexdump(*vvv,vvv));
	terminal_initialise();
	while(1)
	{
    	//if (lines == 25){terminal_initialise();lines = 0;}
    	readcode();
    	unsigned int * j = p;
    	printf(hexdump((p),vvv));
    	printf("\t");
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf(hexdump(*p,vvv));p++;p++;p++;p++;
    	printf("\t\t");
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	terminal_putchar(*j);j++;
    	puts("");
    	lines++;
    }
char* v;
    printf("CLOCKOS\n");
    readcode();
    char vvv[9] = "000000000";
    readcode();
    puts("DISKIO");
    readcode();
    halt();
    struct pci_device dev[8192];
    int i = pciEnumAll(dev);
    for (;i<0;i--) {printf(hexdump(dev[i].bus,vvv));printf(":");puts(hexdump(dev[i].device,vvv));};
    return;
    //while(1){char vv = ; if(vv!=x){io_wait();terminal_putchar(scancode[vv]);}}//printf(" ");puts(itoa(vv,v));};}x=vv	
*/
/*
{
        char v[9] = {0,0,0,0,0,0,0,0,0};
        printf(hexdump(sizeof(HBA_MEM),v));
    }
    puts("DISK IO");
    char vvv[9] = "000000000";
    struct pci_device d[32];
    int i = pciEnumAll(d);
    puts("DONE!");
    printf("Got ");
    printf(hexdump(i,vvv));
    puts(" devices.");
    puts("Select device:");
    puts("N\tBus\t\tDevice\tType\tVendor\tFunction");
    for (int j = i-1;j>=0;j--)
    {
        printf(hexdump(j,vvv)+7);printf("\t");
        printf(hexdump(d[j].bus,vvv)+4);printf("\t");
	printf(hexdump(d[j].device,vvv)+4);printf("\t");
	printf(hexdump(d[j].class,vvv)+4);printf("\t");
	printf(hexdump(d[j].vendor,vvv)+4);printf("\t");
	printf(hexdump(d[j].function,vvv)+4);printf("\t");
	puts("");
    };
    int c = readchar();
    puts(hexdump(c-'0',vvv));
    struct pci_device dev = d[c-'0'];
    FIS_REG_H2D * fis;
    memset(fis, 0, sizeof(FIS_REG_H2D));
    fis->fis_type = FIS_TYPE_REG_H2D;
    fis->command = 0xEC;	// 0xEC
    fis->device = 0;			// Master device
    fis->c = 1;				// Write command register
    printf("Selected ");
    puts(hexdump(((unsigned int) dev.bus<<16|(unsigned int) dev.device<<8|(unsigned int) dev.function << 8),vvv)+2);
    
    HBA_MEM * m = PCI2HBA(dev);
    //dump(pciConfigReadReg(dev.bus,dev.device,dev.function,HBA_MEM_PCI_OFFSET));
    //unsigned char read(HBA_PORT *port, unsigned int startl, unsigned int starth, unsigned int count, unsigned short *buf);
    puts("enum");
    int v[32];
    enumerateHBA_MEM(m,v);
    printf("Dev: ");
    c = readchar();
    puts(&c);
    HBA_PORT * port = (m->ports)+(sizeof(HBA_PORT)*(c-'0'));
    unsigned short* b = 133769;
    unsigned char r = read(port,1,1,16,b);
    puts ("DONE");
    puts(hexdump(r,vvv));
    dump(133769);
*/
uint32_t crc32(uint32_t crc, const char *buf, size_t len)
{
	static uint32_t table[256];
	static int have_table = 0;
	uint32_t rem;
	uint8_t octet;
	int i, j;
	const char *p, *q;
 
	/* This check is not thread safe; there is no mutex. */
	if (have_table == 0) {
		/* Calculate CRC table. */
		for (i = 0; i < 256; i++) {
			rem = i;  /* remainder from polynomial division */
			for (j = 0; j < 8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				} else
					rem >>= 1;
			}
			table[i] = rem;
		}
		have_table = 1;
	}
 
	crc = ~crc;
	q = buf + len;
	for (p = buf; p < q; p++) {
		octet = *p;  /* Cast to unsigned octet. */
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
	}
	return ~crc;
}
typedef struct __attribute__((packed))
{
    unsigned long long sig;
    unsigned char rev[4];
    unsigned int size;
    unsigned int crc;
    unsigned int rsvd1;
    unsigned long long this;
    unsigned long long backup;
    unsigned long long first;
    unsigned long long last;
    unsigned char guid[16];
    unsigned long long part_start;
    unsigned int part_num;
    unsigned int part_ent_size;
    unsigned int part_crc;
    unsigned char rsvd2[420];
} GPT_HEAD;
typedef struct __attribute__((packed))
{
    unsigned char type_guid[16];
    unsigned char part_guid[16];
    unsigned long long first;
    unsigned long long last;
    unsigned char sys:1;
    unsigned char ignore:1;
    unsigned char legacy:1;
    unsigned char rsvd1:5;
    unsigned char rsvd2[5];
    unsigned char specific[3];
    unsigned char name[72];
} GPT_PART;
typedef struct __attribute__((packed))
{
    unsigned char sector:6;
    unsigned short cylinder:10;
    unsigned char head;
} CHS;
#define BAD_CHS {.cylinder = 1023, .head=255, .sector = 63}
typedef struct __attribute__((packed))
{
    unsigned char status;
    CHS start;
    unsigned char type;
    CHS end;
    unsigned int LBA;
    unsigned int sectors;
} MBR_PART;
typedef struct __attribute__((packed))
{
    unsigned char bootstrap[446];
    MBR_PART parts[4];
} MBR_HEAD;
#define EFI_SIG 0x5452415020494645

GPT_HEAD test =
    {
        .sig = EFI_SIG,
        .rev = {0,0,1,0},
        .this = 1,
        .backup = 1024,
        .first = 1024,
        .last = 2048,
        .guid = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x69},
        .part_start = 2,
        .part_num = 0x80
    };
GPT_PART test2 =
    {
        .type_guid = {0x0F,0xC6,0x3D,0xAF,0x84,0x83,0x47,0x72,0x8E,0x79,0x3D,0x69,0xD8,0x47,0x7D,0xE4},
        .part_guid = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .first = 1024,
        .last = 1029,
        .name = "YAY!"
    };
void gen_crc(GPT_HEAD * h)
{
    h->crc = crc32(0,h,512);
}
void gen_part_crc(GPT_HEAD * h, char * p)
{
    h->part_crc = crc32(0,p,h->part_num);
}
GPT_PART test3[80];
MBR_HEAD test4 = {.parts = {{.status = 0x00, .type = 0xEE, .LBA = 1}}};
void kernel_main(void)
{
    gen_crc(&test);
    memcpy(test3,&test2,sizeof(GPT_PART));
    gen_part_crc(&test,test3);
    int i = sizeof(GPT_PART);
    printf("%x",0xAB);
    char vvv[9] = "000000000";
    pciEnumAll();
    if (get_abar()) {puts("No AHCI device. Halting!");halt();}
//    unsigned int * where=133769;
//    cpuid_string(CPUID_GETVENDORSTRING,where);
    //unsigned int v = 0;
    //__get_cpuid_max(0,&v);
    char v[13]="????????????";
    printf ("Running on a %s processor\n", get_vendor(v));
    asm(
        "cli\n"
        //"lgdt [gdtr]\n"
        "mov %eax, %cr0\n"
        "or %eax, 1\n"
        "mov %cr0,%eax\n"
    );
    puts("Protected mode!");
    int ports[32];
    probe_port(ports);
    //puts("Select device:");
    //puts("N\t\t\tType");
    //for (int i = 0;i<32;i++) {if (ports[i] == 0) continue;printf(hexdump(i,vvv));printf("\t");puts(hexdump(ports[i],vvv));}
    puts("Init disk!");
    HBA_PORT * port = &abar->ports[0];
    write(port,1,0,sizeof(test),&test);
    write(port,2,0,(80*sizeof(GPT_PART)),&test3);
    /*
    while (1)
    {
        readchar();
        char c = 0;
        HBA_PORT * port = &abar->ports[c];//+(sizeof(HBA_PORT)*(c-'0'));
        unsigned short b[2];
        unsigned short a[2] = {0x69,0x420};
        printf("%x, %x",a[0],b[0]);
        printf(hexdump(b[0],vvv));
        unsigned char w = write(port,0,0,2,a);
        printf(" -");
        unsigned char r = read(port,0,0,2,b);
        printf("> ");
        printf("%x, %x [%i,%i]\n",a[0],b[0],w,r);
    }
    */
}
