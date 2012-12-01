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
 *  Name: kb.h
 *  External: KB.H
 *  Author: Andrew F. Hart
 *  Date: 2006.05.17
 * =======================================================================
 */

#define GS_KB_BUFSIZE 512

/* Buffer for storing received keypresses. 
 * This is implemented as a circular buffer to
 * avoid the possibility of buffer overflow.
*/
struct
{
  unsigned rindex;		/* read position in the buffer */
  unsigned windex;		/* write position in the buffer */
  unsigned char buf[512];	/* characters stored in buffer */
}kb_buf;

