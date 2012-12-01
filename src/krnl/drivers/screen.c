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
 *  Name:	screen.c
 *  External: screen.o
 *  Author: Andrew F. Hart
 *  Date:	2006.04.08
 * =======================================================================
*/
#include <system.h>


unsigned short *pTextMem;
int		iAttribute = 0x0F;
int		iCursorX = 0, iCursorY = 0;

/* Scrolls the screen */
void scroll(void)
{
	unsigned blank, temp;
	
	/* Blank is defined as a space & therefore needs a bgcolor */
	blank = 0x20 | (iAttribute << 8);

	/* Row 25 is the end, this means we need to scroll up */
 	if(iCursorY >= 25)
	{
		/* Move the current text chunk back in the buffer by one line */
		temp = iCursorY - 25 + 1;	/* Start with line 1 */
		memcpy (pTextMem,pTextMem + temp * 80,(25-temp)*80*2);

		/* Finally, blank out the chunk of memory reppin' the bottom
		 * line with spaces. */
		memsetw(pTextMem + (25-temp)*80,blank,80);
		iCursorY = 25-1;
	}
}

/* Updates the hardware cursor */
void move_cursor(void)
{
	unsigned temp;
	
	temp = iCursorY * 80 + iCursorX;	/* Cursor Position in memory */

	/* Comment taken from  Bran's Kernel Development Tutorial
	 * http://www.osdever.net/bkerndev/index.php?the_id=90
	 *  This sends a command to indicies 14 and 15 in the
    	 *  CRT Control Register of the VGA controller. These
    	 *  are the high and low bytes of the index that show
    	 *  where the hardware cursor is to be 'blinking'. To
    	 *  learn more, you should look up some VGA specific
    	 *  programming documents. A great start to graphics:
    	 *  http://www.brackeen.com/home/vga */
	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

/* Clears the screen */
void cls()
{
	unsigned blank;
	int i;

	blank = 0x20 | (iAttribute << 8);

	/* Set the entire screen to spaces */
	for(i = 0; i < 25; i++)
		memsetw(pTextMem + i * 80, blank, 80);
	
	/* Update virtual cursor and move hardware cursor */
	iCursorX = 0;
	iCursorY = 0;
	move_cursor();
}

/* Puts a single character on the screen */
void putch(unsigned char c)
{
	unsigned short *where;
	unsigned att = iAttribute << 8;
	
	/* Handle a backspace by moving the cursor back one space */
	if(c == 0x08)
	{
		if(iCursorX != 0) iCursorX--;
	}

	/* Handle a tab by incrementing cursor's x, but only
	 * to a point that will make it divisible by 8 */
	else if(c == 0x09)
	{
		iCursorX = (iCursorX + 8) & ~(8-1);	/* <-- thats sweet */
	}

	/* Handle a 'Carriage Return', which just brings the
	 * cursor back to the margin */
	else if (c == '\r')
	{
		iCursorX = 0;
	}
	/* Handle newlines the way DOS and BIOS do.. treat as
	 * if there were an accompanying 'CR'. */
	else if (c == '\n')
	{
		iCursorX = 0;
		iCursorY++;
	}
	/* Any character >= space is a printable character */
	else if (c >= ' ')
	{
		where  = pTextMem + (iCursorY * 80 + iCursorX);
		*where = c | att;
		iCursorX++;
	}
	
	/* If the cursor has reached the edge of the screen,
	 * insert a newline */
	if(iCursorX >= 80)
	{
	  	iCursorX = 0;
		iCursorY++;
	}
	
	/* Scroll the screen if needed, and finally, move the cursor */
	scroll();
	move_cursor();
}

/* Uses putch to output a string */
void puts(unsigned char *text)
{
	int i;
	for(i = 0; i < strlen(text); i++)
	{
		putch(text[i]);
	}
}

/* Sets the forecolor and backcolor to use */
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
	/* Top 4 bytes are bg, bottom 4 bytes are fg */
	iAttribute = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets the text-mode VGA pointer, and clears the screen */
void init_video(void)
{
	pTextMem = (unsigned short *)0xB8000;
	cls();
}
