/**        __  _________  _________  _________ /\
 *      __/\_\/\  _____ \/\  _____ \/\   _____\  \
 *     / /:/ /\:\ \___/\ \:\ \___/\ \:\  \___/_   \
 *    / /:/ / _\:\ \  \:\ \:\ \  \:\ \:\_____  \   \
 *   / /:/ / /\ \:\ \  \:\ \:\ \  \:\ \/___/:\  \   \
 *   \ \:\ \//\\ \:\ \__\:\ \:\ \__\:\ \  _\_:\  \   \
 *    \ \:\//_/\\_\:\________\:\________\/\_______\   \   
 *     \ \:\  \//  \/________/\/________/\/_______/   /
 *      \ \:\ //  / GOOS - GO Operating System       /
 *       \ \://  /   c.2006 Andrew F. Hart          /
 *        \_\/__/__________________________________/
 * 
 * ======================================================================
 *  Name: cons.c
 *  External: CONS.O
 *  Author: Andrew F. Hart
 *  Date: 2006.05.22
 * =======================================================================
 */
#include <system.h>
#include <kb.h>

/* Provides Console IO functions */

/*
 * cls		- clear the window portion of the screen
 * 
 * putc		- print a character on the screen
 * putch	- called by printf, calls putc
 * puts		- print a string on the screen
 * putsp	- print a prefix and string
 * putraw	- prints a char with spec. attr at spec. location
 * 
 * getc		- get a character from the kb buffer
 * getch	- called by scanf and friends, calls getc
 *
*/

/* Video Memory Variables 
*/
unsigned short *pVidmem = (unsigned short *) 0xb8000;
unsigned gs_cons_rows	= 24;
unsigned gs_cons_cols   = 80;


/* Cursor Location Variables
*/
int cursorx;
int cursory;

/* Style Variables
*/
unsigned char gs_mnubar_attr = 0x4F;
unsigned gs_mnubar_blank     = 0x4F20;
unsigned char gs_win_attr    = 0x07;
unsigned gs_win_blank        = 0x0720;


/* Internal Functions
*/
void init_video()
{
    pVidmem = (unsigned short *) 0xb8000;
    memsetw(pVidmem,gs_mnubar_blank,80);
    cls();
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

void scroll()
{
    /* Scroll the screen up one line if needed */
    if(cursory >= 25)
    {
      memcpy(pVidmem + gs_cons_cols,pVidmem + (gs_cons_cols << 1),80*24*2);
      /* Blank out the last line */
      memsetw(pVidmem + ((24) * gs_cons_cols), gs_win_blank, 80);
      cursory = 24;
    }
}


/* Externally Available Functions 
*/
void init_cons()
{
  init_video();	/* Initialize the Video Memory */
  kb_install(); /* Install a keyboard handler on IRQ 1 */ 
}

void cls()
{
    unsigned short i;
    for(i=1;i<25;i++)
    {
       memsetw(pVidmem + i*gs_cons_cols,gs_win_blank,gs_cons_cols);  
    }
    cursorx = 0;
    cursory = 1;
    set_cursor();    	
}

/* - Output Functions
*/
void putc(unsigned char val)
{
    /* PRINTABLE CHARACTERS */
    if(val >= ' ')
    {
      /* Store value in vga memory for display */
      pVidmem[(unsigned)(cursory * gs_cons_cols + cursorx)] = val | (gs_win_attr << 8);
      cursorx++;
    }
    else{
      /* SPECIAL CHARACTERS */
      switch(val)
      {
        /* BACKSPACE:
         * move cursor back one space if possible
        */
        case 0x08: if(cursorx != 0)
                   {
                     cursorx--;
                     pVidmem[(unsigned)(cursory * gs_cons_cols + cursorx)] = 0 | (gs_win_attr << 8);
                   }
                   else
                   {
                     pVidmem[(unsigned)(cursory * gs_cons_cols + cursorx)] = 0 | (gs_win_attr << 8);
                   }
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

void putraw(int x, int y, unsigned char val,unsigned char attr)
{
  unsigned value = val | (attr << 8);
  memsetw(pVidmem + (80*y) + x,value,1);
}

/* - Input Functions
*/ 
unsigned char getc()
{
  if(kb_buf.rindex == kb_buf.windex)
  {
    return (0);		/* no unhandled characters */
  }
  unsigned char c = kb_buf.buf[kb_buf.rindex++];
  if(kb_buf.rindex == GS_KB_BUFSIZE)
  {
    kb_buf.rindex = 0;
  }
  return (c);
}




/* -Called By printf and friends 
*/
void putch(unsigned char c)
{
    putc(c);
}

unsigned char getch()
{
  unsigned char c;
  while((c = getc()) == 0);	/* Wait For Keypress */
  return (c);
}

