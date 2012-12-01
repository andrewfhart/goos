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
 *  Name: strcmp.c
 *  External: strcmp.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.26
 * =======================================================================
 */


int strcmp(const char* a, const char* b)
{
  int alen = 0;
  int blen = 0;
 
  /*printf("Comparing %s and %s: ",a,b);*/

  /* get length of "a" */
  while(a[alen++] != '\0');
  /* get length of "b" */
  while(b[blen++] != '\0');
  /* compare lengths */
  if(alen < blen){return alen;}
  if(alen > blen){return blen;}
  /* compare characters */
  int i;
  for(i=0;i<alen;i++)
  {
    if(a[i] != b[i]){return (i+1);}
  }
  /* the strings are the same */
  return (0);
}
