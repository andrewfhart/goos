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
 *  Name: gooslink.ld
 *  External: GOOSLINK.LD
 *  Author: Andrew F. Hart
 *  Date: 2006.06.15 (replaces /src/krnl/gklink.ld created 2006.04.18)
 * =======================================================================
 *
 */
SEARCH_DIR(src\krnl\dbg\obj);
SEARCH_DIR(src\lib\dbg\obj);

/* KERNEL INPUT FILES */
INPUT("gkstart.o","main.o","cons.o","gdt.o",
      "idt.o","isrs.o","irq.o","pit.o",
      "kb.o","ci.o","cmos.o","mm.o"
      );
/* LIBRARY INPUT FILES */
INPUT("printf.o","readline.o","strcmp.o");
OUTPUT_FORMAT("binary")
ENTRY(gkstart)
phys = 0x00009000;
SECTIONS
{
	.text phys : AT(phys) {
		code = .;
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(phys + (data - code)){
		data = .;
		*(.data)
		. = ALIGN(4096);
	}
	.bss : AT(phys + (bss - code))
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}
	end = .;
}
