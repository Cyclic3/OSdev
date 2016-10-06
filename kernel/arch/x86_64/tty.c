#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>
#include <kernel/io.h>
#include <kernel/tty.h>
#define BADCHAR '\x1a'
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialise(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_GREEN, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
size_t cursor_back_limit = 0;
size_t cursor_pos = 0;
void cursor_to(int index)
{
  outb(0x3D4, 0x0F);
  outb(0x3D5, index);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char )((index>>8)&0xFF));
  cursor_pos = index;
}
void cursor_update()
{
  const size_t index = terminal_row * VGA_WIDTH + terminal_column + 1;
  outb(0x3D4, 0x0F);
  outb(0x3D5, index);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char )((index>>8)&0xFF));
  cursor_pos = index;
}
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
        cursor_to(index+1);
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
	//Why was this nested? --C3
	if ( terminal_row == VGA_HEIGHT)
	{
		memmove(terminal_buffer,terminal_buffer+VGA_WIDTH,VGA_HEIGHT*VGA_WIDTH*2);//*2 to account for the tuple --C3
		//memset(terminal_buffer + (VGA_HEIGHT*VGA_WIDTH),0,256);
		terminal_column = 0;
		terminal_row--;
	}
	if ( terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		terminal_row += 1;
	}
	

	if (c=='\n'){terminal_column = 0;terminal_row = terminal_row + 1;return cursor_update();}
	if (c=='\t'){terminal_column = ((terminal_column+4)/4)*4; return cursor_update();}
	if (c=='\b')
	{
	  if (cursor_pos > cursor_back_limit)
	  {
	    terminal_putentryat(' ',terminal_color, --terminal_column, terminal_row);
	    cursor_to(cursor_pos-1);
	    return;
	  }
	  else return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	++terminal_column;
	
	
	
}

void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
void clearline()
{
	terminal_column = 0;
	for (int i = 0; i < 80; i++){terminal_putentryat(' ', terminal_color, i, terminal_row);}
}
void writeline(char* v) {terminal_writestring(v);}
Scancode readcode() {return getScancode();}
char readchar()
{
  Scancode s = readcode();
  if (s.Shift)
  {return scancode_shift[s.code];}
  else
  {return scancode[s.code];}
}
char* readline(char* acc,char echo, int n)
{
  cursor_back_limit = cursor_pos;
  int pos = 0;
  while (pos < n)
  {
    char v = readchar();
    if (echo) {terminal_putchar(v);};
    if (v=='\n'){acc[pos]='\0';return acc;};
    if (v=='\b' && cursor_pos > cursor_back_limit ) {acc[pos--]='\0';}
    else acc[pos++] = v;
    n--;
  }
  return acc;
}
