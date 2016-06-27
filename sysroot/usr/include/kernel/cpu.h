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
#include <cpuid.h>
char * get_vendor(char * s)
{
    unsigned int a = 0;
    unsigned int b,c,d = 0;
    if(!__get_cpuid(0,&a,&b,&c,&d)) puts("=(");
    else {memcpy(s,&b,4);memcpy(s+4,&d,4);memcpy(s+8,&c,4);}//memcpy(s+12,&d,4);}
    return s;
}
