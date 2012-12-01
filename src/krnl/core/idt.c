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
 *  Name: idt.c
 *  External: idt.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.13
 * =======================================================================
 */
#include <system.h>


/*
 * Purpose of this file:
 * This file provides implementation for the construction and 
 * installation of the Interrupt Descriptor Table (IDT) structure. 
 * The Interrupt Service Routines (ISRs) that are executed as 
 * a result of an IDT lookup are contained in (/krnl/core/isrs.c).
 *
 * The IDT
 * The IDT is a data structure similar to the GDT (/krnl/core/gdt.c)
 * in that it provides a lookup table for the processor. Instead
 * of looking up memory ranges, it looks up handlers for particular
 * interrupts. An interrupt occurs when the PIC enables the interrupt
 * pin on the processor. The processor interrupts whatever it was doing
 * to service the interrupt and then resumes exactly where it left off.
 * Interrupts may be caused by the processor itself (in the case of a 
 * divide by zero error, for example) or by external devices (such as
 * the keyboard signalling that a key has been pressed). For all cases,
 * a short routine called an Interrupt Service Routine is defined which
 * is called to process a specific interrupt. These are itemized here,
 * in 64-bit entries in the IDT, and defined in (/krnl/core/isrs.c).
 *
 * Installing the IDT is also similar to installing the GDT: a special
 * 48 bit pointer specifying the size (16bits) and base address (32bits)
 * of the IDT is passed using the lidt assembly instruction.
 *
 * IDT Entry Breakdown
 *  15-00 Bits 15 to 0 of the base address
 *  31-16 Kernel segment address
 *  39-32 Always 0
 *  47-40 Access Flags field (see special comment below)
 *  63-48 Bits 31 to 16 of the base address
 *
 *  Access Flags:
 *  +----+----+
 *  |PDD0|1110|
 *  +----+----+
 *   P:  Presence bit (is the segment present?) 1 = yes
 *       If the presence bit is cleared, the entry will cause an
 *       'unhandled interrupt' exception
 *   DD: Privilege (highest ring (0-3) allowed use of the interrupt)
 *       (Ring 0 is the ring with highest privilege)
 *   01110: The lower bits of the Access Flags Byte are always set
 *          to 14.
 *
*/

/* The definition of a 64 bit IDT entry */
struct idt_entry
{
  unsigned short base_low;	/* EIP 15:0 (lower bytes of handler offset addr) */
  unsigned short sel;		/* CS  15:0 (handler selector) */
  unsigned char  zero;		/* Always zero */
  unsigned char  flags;		/* Access Byte (0x8E,0x8F,0xEE,0xEF) */
  unsigned short base_high;	/* EIP 31-16 (upper bytes of handler offset addr) */
} __attribute__((packed));

/* The Special 48-bit structure defining the size and address of the
 * IDT.
*/
struct idt_ptr
{
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

/* The IDT */
struct idt_entry idt[256];
struct idt_ptr pIDT;

/* Sets the values of an IDT entry. This is very similar to the way
 * the GDT entries were set in gdt_set_gate (/krnl/core/gdt.c).
*/
void idt_set_gate(unsigned char num, 
                  unsigned long base, 
                  unsigned short sel, 
                  unsigned char flags)
{
   idt[num].base_low  = base & 0xFFFF;
   idt[num].sel	      = sel;
   idt[num].zero      = (unsigned char) 0x00;
   idt[num].flags     = flags;
   idt[num].base_high = (base >> 16) & 0xFFFF;  
}



/* Installation of the IDT. This function creates the IDT entries
 * and assigns the pointer to the IDT structure using the 
 * lidt assembly instruction
*/
void idt_install()
{
   /* Create the special pointer */
   pIDT.limit = (sizeof(struct idt_entry) * 256) - 1;
   pIDT.base  = (int) &idt;

   /* Clear the IDT */
   memset(&idt,0,sizeof(struct idt_entry) * 256);
   
   /* IDT Entries using idt_set_gate */

   /* Load the IDT*/
   idt_load();
}








