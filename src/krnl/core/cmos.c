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
 *  Name: cmos.c
 *  External: cmos.o
 *  Author: Andrew F. Hart
 *  Date: 2006.06.13
 * =======================================================================
 */
#include <system.h>

/* read_cmos() - reads the specified register in the CMOS
 *
*/
unsigned read_cmos(unsigned reg)
{
  outportb(0x70,reg);
  return inportb(0x71);
}


/* write_cmos() - writes a value to the CMOS at the offset
 * 		  specified by offset
 *
void write_cmos(unsigned offset,unsigned reg)
{

}
 *
 *
*/

