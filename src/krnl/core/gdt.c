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
 *  Name: gdt.c
 *  External: gdt.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.13
 * =======================================================================
 */

#include <system.h>

/* Purpose of this file:
 * This file provides implementation for the construction and 
 * installation of the Global Descriptor Table (GDT) structure. 
 * It duplicates the GDT that was constructed in assembly
 * (/boot/sector2.asm) for use in making the initial jump to 32-bit
 * protected mode. The GDT is being re-installed by the kernel
 * so that the kernel knows exactly where the GDT is (in case the 
 * boot code changes) and will never overwrite it. Also, modifications
 * to the GDT are more easily made in C than in assembly. 
 * 
 * The GDT
 * The GDT is a data structure that describes the portions of memory
 * that code and data are allowed to access. It is also possible
 * to provide additional entries in the GDT beyond those for code and 
 * data (one for video memory for example). The GDT is a table of 
 * 64 bit entries with a predefined format:
 *
 * GDT ENTRY BREAKDOWN
 *  15-00 Bits 15-0 of the segment limit (FFFF = 4GB)  
 *  31-16 Bits 15-0 of the base address  (0000 = 0h)
 *  39-32 Bits 23-16 of the base address (00 = 0h)
 *  40    Access Bit (for use with virtual memory) = 0
 *  43-41 Descriptor Type
 *          43: Executable (0=data segment, 1=code segment)
 *          42-data: exapansion direction (0=expandup)
 *          42-code: conforming
 *          41-data: Read/Write: 
 *                   (0=RO, 1=RW)
 *          41-code: Read/Execute: 
 *                   (0=Execute Only, 1=Read/Execute)
 *  44    Descriptor Bit:
 *                   (0=system desc., 1=code/data desc)
 *  46-45 Descriptor privilege level 
 *                   (0=highest, 3=lowest) (Ring0,Ring3)
 *  47	  Present Bit 
 *                   (segment present in memory (virtual mem stuff))
 *  51-48 Bits 19-16 of segment limit
 *  52    Reserved for the OS
 *  53    Reserved, should be 0
 *  54    16/32bit segment 
 *        (0=16bit, 1=32bit);
 *  55    Granularity 
 *        (1=multiply limit by 4k)
 *  63-56 Bits 31-24 of the base address (00 = 0h)
 *
 * GDT ENTRIES:
 *  NULL DESCRIPTOR: 
 *  0x00000000 0x00000000
 *  CODE DESCRIPTOR:
 *  0x00cf9c00 0x0000ffff
 *  DATA DESCRIPTOR:
 *  0x00cf9200 0x0000ffff 
 *  VIDEO DESCRIPTOR:
 *  0x0000920b 0x80003999 (4000 bytes of video mem starting at 0xb8000)
 * 
 *
*/

#define num_gdt_entries  4

/* Definition of a GDT entry. Fields are described in 
 * greater detail above.
*/
struct gdt_entry
{
  unsigned short limit_low;     /* bits 15-00 */
  unsigned short base_low;      /* bits 31-16 */ 
  unsigned char base_middle;    /* bits 39-32 */
  unsigned char access;         /* bits 47-40 */
  unsigned char granularity;    /* bits 55-48 */
  unsigned char base_high;      /* bits 63-55 */
} __attribute__((packed));

/* Special 48 bit pointer to the GDT. The structure specifies the size 
 * (16 bits) and the base address (32 bits) of the GDT. The GDT is 
 * installed by providing an instance of this structure to the 
 * lgdt assembly instruction. 
*/
struct gdt_ptr
{
  unsigned short limit;		/* size of the GDT */
  unsigned int base;		/* Location of GDT start */
} __attribute__((packed));

/* The GDT itself
*/
struct gdt_entry gdt[num_gdt_entries];
struct gdt_ptr pGDT;

void gdt_set_gate(int num,
                    unsigned long base, 
                    unsigned long limit, 
                    unsigned char access, 
                    unsigned char granularity)
{
  gdt[num].base_low    = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high   = (base >> 24) & 0xFF;

  gdt[num].limit_low   = (limit & 0xFFFF);
  gdt[num].granularity = (limit >> 16) & 0x0F;
  
  gdt[num].granularity |= granularity & 0xF0;
  gdt[num].access      = access;
}

void gdt_install()
{
  /* Set GDT pointer and limit */
  pGDT.limit = (sizeof(struct gdt_entry) * num_gdt_entries) - 1;
  pGDT.base  = (int) &gdt;

  /* NULL DESCRIPTOR */
  gdt_set_gate(0,0,0,0,0);
  
  /* CODE SEGMENT DESCRIPTOR */
  gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);
  
  /* DATA SEGMENT DESCRIPTOR */
  gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);
  
  /* VIDEO SEGMENT DESCRIPTOR */
  gdt_set_gate(3,0xB8000,0x3999,0x92,0x00);
  
  /* Flush old GDT and install changes */
  gdt_flush();
}



