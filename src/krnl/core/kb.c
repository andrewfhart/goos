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
 *  Name: kb.c
 *  External: KB.O
 *  Author: Andrew F. Hart
 *  Date: 2006.05.17
 * =======================================================================
 */
#include <system.h>
#include <kb.h>


unsigned char kbd_xt[128] = 
{ 0,
  0,
  '1','2','3','4','5','6','7','8','9','0','-','=',
  0x08, /* BACKSPACE */
  0x09, /* TAB */
  'q','w','e','r','t','y','u','i','o','p','[',']',
  '\n', /* ENTER */
  0,
  'a','s','d','f','g','h','j','k','l',';','\'',
  0,
  0,    /* LEFT SHIFT */
  0,
  'z','x','c','v','b','n','m',',','.','/',
  0,  /* LEFT SHIFT (k:57, sc: 0x36) */
  0,
  0,  /* LEFT ALT (k:60, sc: 0x38) */
  ' ',/* SPACE BAR(k:61, sc: 0x39) */
  0,
  0,
  0,    
};

/* Interrupt Handler
*/
void kb_handler(struct regs* r)
{
  unsigned char scancode;
  scancode = inportb(0x60);

  if(scancode & 0x80)
  {
     /* key just released (top bit of the byte set) */
  }
  else
  {
     kb_buf.buf[kb_buf.windex++] = kbd_xt[scancode];
     if(kb_buf.windex == GS_KB_BUFSIZE){kb_buf.windex = 0;}
  }
}	

void kb_install()
{
  irq_install_handler(1,kb_handler);
}
