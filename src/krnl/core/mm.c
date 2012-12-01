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
 *  Name: mm.c
 *  External: MM.O
 *  Author: Andrew F. Hart
 *  Date: 2006.06.16
 * =======================================================================
 */
#include <system.h>

/*
 * Memory Management Code
 *
*/


/* Memory Mapping
*/
unsigned long *page_directory = (unsigned long *) 0x9C000;
unsigned long *page_table = (unsigned long *) 0x9D000;

unsigned long address = 0;
unsigned int i;

void map_mem()
{
  /*map the first 4MB of memory */
  for(i=0;i<1024;i++)
  {
    page_table[i] = address | 0x3; /* supervisor level, r/w, present */
    address += 4096;		   /* 4096 = 4kb */ 
  }

  page_directory[0] = (unsigned long) page_table;
  page_directory[0] |= 3;

  for(i=1;i<1024;i++)
  {
     page_directory[i] = 0 | 0x2;  /* superviser level, r/w, NOT present */
  }
}

/* Paging Initialization
*/
void init_paging()
{
  map_mem();
  printf("before: %b,%b ---",read_cr0(),read_cr3());
  write_cr3((unsigned long)page_directory);
  unsigned long cr0 = read_cr0();
  cr0 = cr0 | 0x8000000;
  write_cr0(cr0);
  printf(" after: %b,%b\n",read_cr0(),read_cr3());
}

