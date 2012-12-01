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
 *  Name: printf.c
 *  External: printf.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.24
 * =======================================================================
 */
#include <system.h>
#include <stdarg.h>


/* Simplified Printf
 *
 * flags implemented:
 *      - %c:   character
 *      - %s:   string
 * 	- %d: 	integer value
 *	- %b:	binary value
 *
*/
void vprintf(const char * format,va_list ap)
{
  /* do the processing of the format string here */
  while(*format != '\0')
  {
    if(*format == '%')
    {
      unsigned char flag = *(format+1);
      char buf[32];
      int ival = 0;
      switch (flag)
      {
        case 'b': ival = va_arg(ap,int);
                  itob(ival,buf,32);
                  puts(buf);
                  format += 2; /* skip the %b */
                  break;
        case 'c': putch(va_arg(ap,char));
                  format += 2; /* skip the %c */
                  break;
        case 'd': ival = va_arg(ap,int);
                  itoa(ival,buf,32);
                  puts(buf);
                  format += 2; /* skip the %d */
                  break;
        case 's': puts(va_arg(ap,char*));
                  format += 2; /* skip the %s */
                  break;                 
        default:  break;
      } 
    }
    else
    {
      putch(*format);	    /* just print the character */
      format++;
    }
  }
}

void printf(const char* format,...)
{
  va_list ap;
  va_start(ap,format);
  vprintf(format,ap);
  va_end(ap);  
}

void itob(int val,char* buf,int bufsize)
{
  char bits[bufsize];
  int a; 
  for(a=0;a<bufsize;a++){bits[a] = '\0';}

  /* divide by 2 and store modulus,
   * finally, reverse order and return 
  */
  int i=0;
  while (val > 0)
  {
    bits[i] = (char)((val % 2) + 0x30);
    val = val >> 1;
    i++;
  }
  int startflag = 0;  
  int j=0;
  for(i=bufsize-1;i>=0;i--)
  {
    if(!startflag && bits[i] > '\0'){startflag = 1;}
    if(startflag)
    {
      buf[j] = bits[i];
      j++;
    }    
  }
  buf[j] = '\0';  
}

