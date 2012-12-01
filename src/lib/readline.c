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
 *  Name: readline.c
 *  External: readline.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.27
 * =======================================================================
 */
#include <system.h>

#define GS_LINEBUF_LENGTH 1024
static char linebuf[GS_LINEBUF_LENGTH];

/*
 * READLINE
 * - reads characters from the console and returns when 
 *   it encounters a \n. 
 *
*/

char* readline(char* prompt)
{
  if(prompt)
  {
    printf("%s",prompt);
  }
  int i=0;
  for(;;)
  {
    if(i < GS_LINEBUF_LENGTH)
    { 
       linebuf[i] = getch();	/* get a character from the kb_buf */
       putch(linebuf[i]);	/* echo to the terminal */
       if(linebuf[i] == '\n')	/* stop when encounter newline */
       {
         linebuf[i] = '\0';	/* replace \n with null terminator */
         break;
       }
       if(linebuf[i] == 0x08)
       {
         linebuf[i] = 0; i--;	/* remove backspace signal */
         if(i>=0)
         {
           linebuf[i] = 0; i--; /* remove backspaced character */
         }
       }
       i++;
    } 
    else
    {
      linebuf[i-1] = '\0'; 	/* buffer full, place null terminator */
      break;			/* and return */
    }
  }
  return (linebuf);     /* return buffer */
}
