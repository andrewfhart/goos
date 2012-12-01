/*        __  _________  _________  _________ /\
 *     __/\_\/\  _____ \/\  _____ \/\   _____\  \
 *    / /:/ /\:\ \___/\ \:\ \___/\ \:\  \___/_   \
 *   / /:/ / _\:\ \  \:\ \:\ \  \:\ \:\_____  \   \
 *  / /:/ / /\ \:\ \  \:\ \:\ \  \:\ \/___/:\  \   \
 *  \ \:\ \//\\ \:\ \__\:\ \:\ \__\:\ \  _\_:\  \   \
 *   \ \:\//_/\\_\:\________\:\________\/\_______\   \   
 *    \ \:\  \//  \/________/\/________/\/_______/   /
 *     \ \:\ //  / GOOS - GO Operating System       /
 *      \ \://  /   c.2006 Andrew F. Hart          /
 *       \_\/__/__________________________________/
 *
 * ======================================================================
 *  Name: vgax.c
 *  External: vgax.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.20
 * =======================================================================
*/
#include <system.h>

/*
 * Purpose of this driver:
 * This driver provides the programmer with a level of abstraction in
 * dealing with character output to the screen. It eliminates the need
 * to directly access video memory by physical address. The driver
 * provides the following functions:
 * 	- void cls()
 *	  clears the screen and replaces the cursor to position 0,0
 *	- void 
 *
 * VGA Video Memory
 * VGA video memory is laid out as a linear array of 4000 bytes of memory
 * beginning at address 0x000b8000. The memory is logically organized in 
 * 2 byte (16 bit) words with one byte representing the forground (text) 
 * color and the other the background color of a character at each location 
 * on the screen. The screen itself is divided into 25 rows of 80 
 * characters each: 
 * 80 x 25 = 2000 characters * 2 bytes / character = 4000 bytes.
 *
 *  ATTRIBUTE CHARACTER
 * +----+----+----+----+
 * |0000 0000 0000 0000| = 16 bits
 * +----+----+----+----+
 *  bgc  txtc asciichar
 * 
 * Values: Background Color (bgc)..: 0-7 (msb specifies "blinking")
 *         Text Color (txtc).......: 0-15
 *         Text (asciichar)........: 0-255
 *
*/


unsigned short *pVidmem;
unsigned char cursorx;
unsigned char cursory;
unsigned char bgcolor;
unsigned char color;

void init_video()
{
    pVidmem = (unsigned short *) 0xb8000;
    bgcolor = (unsigned char) 0x0;
    color   = (unsigned char) 0x7;
    
    memsetw(pVidmem,0x4F20,80);
    cls();
}

void set_color(unsigned char val)
{
    color = val;
}

void set_bgcolor(unsigned char val)
{
    bgcolor = val;
}

void set_attribute(unsigned char val)
{
    color   = (unsigned char) val & 0x0F;
    bgcolor = (unsigned char) ((val << 4) & 0xF0);    
}

void set_cursor()
{
    unsigned index = cursory * 80 + cursorx;
    /* 
     * Send a command to indices 14,15 in the CRT Control
     * Register of the VGA controller. The data corresponds
     * to the high and low bytes of the index marking the
     * location where the cursor is to be displayed.
    */
    outportb(0x3d4, 14);
    outportb(0x3d5, index >> 8);
    outportb(0x3d4, 15);
    outportb(0x3d5, index);    
}


void cls()
{
    unsigned short i;
    unsigned char attr  = (bgcolor << 4) | (color & 0x0F);
    unsigned blank = 0x20 | (attr << 8);
    for(i=1;i<25;i++)
    {
       memsetw(pVidmem + i*80,blank,80);  
    }
    cursorx = 0;
    cursory = 1;
    set_cursor();    	
}

void scroll()
{
    /* Scroll the screen up one line if needed */
    if(cursory >= 25)
    {
      memcpy(pVidmem+80,pVidmem + 160,80*23*2);
      /* Blank out the last line */
      unsigned short blank = 0x20 | ((color | bgcolor << 4) << 8);
      memsetw(pVidmem+80*(25-1),blank,80);
      cursory = 24;
    }
}

void putraw(int x, int y, unsigned char val,unsigned char attr)
{
  unsigned value = val | (attr << 8);
  memsetw(pVidmem + (80*y) + x,value,1);
}

void putc(unsigned char val)
{
    /* PRINTABLE CHARACTERS */
    if(val >= ' ')
    {
      /* Store value in vga memory for display */
      pVidmem[(unsigned)(cursory*80 + cursorx)] = val | ((color | bgcolor << 4) << 8);
      cursorx++;
    }
    else{
      /* SPECIAL CHARACTERS */
      switch(val)
      {
        /* BACKSPACE:
         * move cursor back one space if possible
        */
        case 0x08: if(cursorx != 0){cursorx--;}
                   break;
        /* TAB:
         * move cursor forward to a location divisible by 8
        */
        case 0x09: cursorx = (cursorx + 8) & ~(8-1); /*<-- thats sweet */
                   break;
        /* CARRIAGE RETURN
         * move cursor back to left margin
        */
        case '\r': cursorx = 0;
                   break;
        /* NEWLINE
         * handle in manner of DOS and BIOS (insert carriage return)
        */
        case '\n': cursorx = 0;
                   cursory++;
                   break;       
      }
    }
    /* If cursor is at righthand edge of screen, go to next line */
    if(cursorx >= 80)
    {
      cursorx = 0;
      cursory++;
    }

    /* If cursor is on bottom row, scroll display up one line */
    if(cursory >= 25)
    {
       scroll();
    }

    /* Update cursor position */
    set_cursor();
    return;      
}

void puts(char* val)
{
  unsigned int i = 0;
  while(val[i] != '\0')
  {
    putc(val[i++]);
  }
}

void putsp(char* pfx, char* val)
{
  puts(pfx);
  puts(val);
}

void display_clock(int ticks)
{
  int hsec = ticks % 100;
  int sec  = (ticks / 100) % 60;
  int min  = (ticks / 6000) % 60;
  int hour = (ticks / 360000) % 60; 

  /**
   * CLOCK FORMAT: 
   * 00:00:00.00
   * h  m  s  hs
   *
   * Location: top row (row 0) xpos 79
   *
  */
  putraw(79, 0, (hsec % 10) + 0x30, 0x4f);
  putraw(78, 0, (hsec / 10) + 0x30, 0x4f);
  
  putraw(77, 0, '.', 0x4f);
  putraw(76, 0, (sec % 10) + 0x30, 0x4f);
  putraw(75, 0, (sec / 10) + 0x30, 0x4f);
  
  putraw(74, 0, ':', 0x4f);
  putraw(73, 0, (min % 10) + 0x30, 0x4f);
  putraw(72, 0, (min / 10) + 0x30, 0x4f);

  
  putraw(71, 0, ':', 0x4f);
  putraw(70, 0, (hour % 10) + 0x30, 0x4f);
  putraw(69, 0, (hour / 10) + 0x30, 0x4f);
}


