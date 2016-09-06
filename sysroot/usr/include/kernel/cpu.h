/* Vendor-strings. */
#define CPUID_VENDOR_OLDAMD       "AMDisbetter!" //early engineering samples of AMD K5 processor
#define CPUID_VENDOR_AMD          "AuthenticAMD"
#define CPUID_VENDOR_INTEL        "GenuineIntel"
#define CPUID_VENDOR_VIA          "CentaurHauls"
#define CPUID_VENDOR_OLDTRANSMETA "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA    "GenuineTMx86"
#define CPUID_VENDOR_CYRIX        "CyrixInstead"
#define CPUID_VENDOR_CENTAUR      "CentaurHauls"
#define CPUID_VENDOR_NEXGEN       "NexGenDriven"
#define CPUID_VENDOR_UMC          "UMC UMC UMC "
#define CPUID_VENDOR_SIS          "SiS SiS SiS "
#define CPUID_VENDOR_NSC          "Geode by NSC"
#define CPUID_VENDOR_RISE         "RiseRiseRise"
#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP
#define IA32_APIC_BASE_MSR_ENABLE 0x800
#define APIC_APICID	 0x20
#define APIC_APICVER	 0x30
#define APIC_TASKPRIOR	 0x80
#define APIC_EOI	 0x0B0
#define APIC_LDR	 0x0D0
#define APIC_DFR	 0x0E0
#define APIC_SPURIOUS	 0x0F0
#define APIC_ESR	 0x280
#define APIC_ICRL	 0x300
#define APIC_ICR	 0x310
#define APIC_LVT_TMR	 0x320
#define APIC_LVT_PERF	 0x340
#define APIC_LVT_LINT0	 0x350
#define APIC_LVT_LINT1	 0x360
#define APIC_LVT_ERR	 0x370
#define APIC_TMRINITCNT	 0x380
#define APIC_TMRCURRCNT	 0x390
#define APIC_TMRDIV	 0x3E0
#define APIC_LAST	 0x38F
#define APIC_DISABLE	 0x10000
#define APIC_SW_ENABLE	 0x100
#define APIC_CPUFOCUS	 0x200
#define APIC_NMI	 (4<<8)
#define TMR_PERIODIC	 0x20000
#define TMR_BASEDIV	 (1<<20)
#define COUNTDOWN_DONE_MSG 1

// Delivery Mode
#define ICR_FIXED                       0x00000000
#define ICR_LOWEST                      0x00000100
#define ICR_SMI                         0x00000200
#define ICR_NMI                         0x00000400
#define ICR_INIT                        0x00000500
#define ICR_STARTUP                     0x00000600

// Destination Mode
#define ICR_PHYSICAL                    0x00000000
#define ICR_LOGICAL                     0x00000800

// Delivery Status
#define ICR_IDLE                        0x00000000
#define ICR_SEND_PENDING                0x00001000

// Level
#define ICR_DEASSERT                    0x00000000
#define ICR_ASSERT                      0x00004000

// Trigger Mode
#define ICR_EDGE                        0x00000000
#define ICR_LEVEL                       0x00008000

// Destination Shorthand
#define ICR_NO_SHORTHAND                0x00000000
#define ICR_SELF                        0x00040000
#define ICR_ALL_INCLUDING_SELF          0x00080000
#define ICR_ALL_EXCLUDING_SELF 0x000c0000

#define IDT_STDINT        0xE
#define IDT_TRPINT        0xf
#define IDT_TSKINT        0x5

#define IOAPIC_ID         0x0
#define IOAPIC_VER        0x1
#define IOREDTBL 0x10

#define SEG_DATA_RD        0x00 // Read-Only
#define SEG_DATA_RDA       0x01 // Read-Only, accessed
#define SEG_DATA_RDWR      0x02 // Read/Write
#define SEG_DATA_RDWRA     0x03 // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04 // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05 // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06 // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07 // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08 // Execute-Only
#define SEG_CODE_EXA       0x09 // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0A // Execute/Read
#define SEG_CODE_EXRDA     0x0B // Execute/Read, accessed
#define SEG_CODE_EXC       0x0C // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0D // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0E // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0F // Execute/Read, conforming, accessed
#define SEG_TSS            0x89

unsigned char * LOCAL_APIC_ADDR;

#include <cpuid.h>

char * get_vendor(char * s)
{
    unsigned int a = 0;
    unsigned int b,c,d = 0;
    if(!__get_cpuid(0,&a,&b,&c,&d)) puts("=(");
    else {memcpy(s,&b,4);memcpy(s+4,&d,4);memcpy(s+8,&c,4);}//memcpy(s+12,&d,4);}
    return s;
}
void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
   asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}
void cpuSetMSR(uint32_t msr, uint32_t lo, uint32_t hi)
{
   asm volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}
typedef struct __attribute__((packed))
{
   unsigned int offset;
   unsigned short selector; 
   unsigned char  type_attr; 
}IDT;
typedef struct
{
  unsigned char present:1;
  unsigned char privilege:2;
  unsigned char set_to_1;
  unsigned char executable:1;
  unsigned char direction:1;
  unsigned char writable:1;
  unsigned char accessed:1;
}type_flags;
typedef union 
{
  type_flags flags;
  unsigned char byte;
} GDT_flags;
typedef struct __attribute__((packed))
{
    unsigned int base;
    unsigned int limit;
    GDT_flags type;
}GDT;
void WriteIDT(unsigned char * target, IDT source)
{
    target[0] = source.offset & 0xFF;
    target[1] = (source.offset>>8) & 0xFF;
    target[2] = source.selector & 0xFF;
    target[3] = (source.selector >> 8) & 0xFF;
    target[4] = 0;
    target[5] = source.type_attr;
    target[6] = (source.offset>>16) & 0xFF;
    target[7] = (source.offset>>24) & 0xFF;
}
void WriteAtIDT(unsigned long long * target, IDT source, unsigned int pos)
{
    printf("[IDT %i] ", pos);
    WriteIDT(&target[pos],source);
    printf("Inserted\n");
}
void WriteGDT(unsigned char * target, GDT source)
{
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && (source.limit & 0xFFF) != 0xFFF) {
        failwith("GDT insertation failed: bad limit\n");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;
 
    // Encode the base 
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // And... Type
    target[5] = source.type.byte;
}
void WriteAllGDT(unsigned char * target, GDT source[], int n)
{
  for (int i = 0;i<n;i++)
  {
      printf("[GDT %i] ",i);
      WriteGDT((unsigned char *)((size_t)target+(i*8)),source[i]);
      printf ("Inserted\n");
  }
}
typedef struct __attribute__((packed))
{
  unsigned short length;
  void * ptr;
} DescriptorPointer;
void LoadGDT(DescriptorPointer i) {asm volatile("lgdt %0"::"m"(i));}
void LoadIDT(DescriptorPointer i) {asm volatile("lidt %0"::"m"(i));}
/*
unsigned int ReadLocalApic(unsigned int reg)
{
    return *((unsigned int *)(LOCAL_APIC_ADDR+reg));
}
void WriteLocalApic(unsigned int reg, unsigned int value)
{
    *((unsigned int *)(LOCAL_APIC_ADDR+reg)) = value;
}
char check_apic() {
  unsigned int eax,ebx,ecx, edx;
   cpuid(1, &eax, &ebx, &ecx, &edx);
   return edx & CPUID_FLAG_APIC;
}
void SetLocalApicBase(unsigned int apic) {
   unsigned int edx = 0;
   unsigned int eax = (apic & 0xfffff100) | IA32_APIC_BASE_MSR_ENABLE;
 
#ifdef __PHYSICAL_MEMORY_EXTENSION__
   edx = (apic >> 32) & 0x0f;
#endif
 
   cpu_set_msr(IA32_APIC_BASE_MSR, eax, edx);
}
unsigned int * GetLocalApicBase() {
   unsigned int eax, edx;
   cpu_get_msr(IA32_APIC_BASE_MSR, &eax, &edx);
 
#ifdef __PHYSICAL_MEMORY_EXTENSION__
   return (eax & 0xfffff000) | ((edx & 0x0f) << 32);
#else
   return (eax & 0xfffff000);
#endif
}
void InitLocalApic()
{
    LOCAL_APIC_ADDR = GetLocalApicBase();
    cpu_set_apic_base(cpuGetAPICBase());
    WriteLocalApic(APIC_SPURIOUS,ReadLocalApic(APIC_SPURIOUS)|0xFF);
    
}
typedef struct __attribute__((packed))
{
    unsigned char vector;
    unsigned char dest_mode:3;
    unsigned char logical_destination:1;
    unsigned char delivery_status:1;
    unsigned char reserved_1:1;
    unsigned char init_deassert_clear:1;
    unsigned char init_deassert_set:1;
    unsigned char destination_type:2;
    unsigned short reserved_2:12;
} RedirectionEntry;
unsigned int LocalApicGetId()
{
    //return ReadLocalApic(APIC_APICID);
}
void AddRedirectionEntry(RedirectionEntry red, unsigned int vector)
{
    unsigned int id = LocalApicGetId();
    WriteLocalApic(APIC_ICR,id);
    LocalApicOut(APIC_ICRL, vector | ICR_STARTUP | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);
    while (ReadLocalApic(APIC_ICRL) & ICR_SEND_PENDING);
}
*/
