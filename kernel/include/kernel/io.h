#ifndef IO_KERN
#define IO_KERN
#include <stddef.h>
#include <stdint.h>
struct pci_device{unsigned char bus;unsigned char device;unsigned short class;unsigned short vendor;unsigned char function;};
const char scancode[256];
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
char getScancode(void);
char ask_cmos(char reg);
void reboot (void);
void outl(uint16_t port, unsigned int val);
unsigned int inl(uint16_t port);
uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset);
unsigned int pciConfigReadReg(uint8_t bus, uint8_t slot,uint8_t func, uint8_t offset);
void pciEnumAll();
struct pci_device devices[32];
int n_devices;
#endif
