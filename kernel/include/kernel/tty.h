#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <kernel/io.h>

void terminal_initialise(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
Scancode readcode(void);
void clearline(void);
char readchar(void);
char* readline(char* acc,char echo, int n);
#endif
