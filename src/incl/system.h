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
 *  Name: system.h
 *  External: SYSTEM.H
 *  Author: Andrew F. Hart
 *  Date: 2006.04.08
 * =======================================================================
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

/* regs
 * Stores a snapshot (stack frame) of the internal state when
 * an ISR has been called. see (/krnl/core/isrs.c),(/krnl/gkstart.asm)
 *
*/
struct regs
{
  unsigned int gs,fs,es,ds;			/* Segment Descriptors */
  unsigned int edi,esi,ebp,esp,ebx,edx,ecx,eax;	/* Registers */
  unsigned int int_no, err_code;		/* Error num (or dummy 0) */
  unsigned int eip,cs,useresp,ss;		/* More registers*/
};

/* TODO: Include control registers cr0-cr3 */
struct cpu
{
  unsigned int gs,fs,es,ds;
  unsigned int edi,esi,ebp,esp,ebx,edx,ecx,eax;
  unsigned int eip,cs,useresp,ss;   
};

/* /KRNL/GKSTART.ASM */
extern void reboot();
extern void gdt_flush();
extern void idt_load();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern unsigned long read_cr0();
extern unsigned long read_cr1();
extern unsigned long read_cr2();
extern unsigned long read_cr3();
extern void write_cr0(unsigned long);
extern void write_cr1(unsigned long);
extern void write_cr2(unsigned long);
extern void write_cr3(unsigned long);
extern void dump_cpu();



/* /KRNL/MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);
extern void itoa(int,char*,int);
extern int atoi(char*);

/* /KRNL/CI.C */
extern void ci();

/* /KRNL/CORE/CONS.C */
extern void init_cons();
extern void set_cursor();
extern void cls();
extern void putc(unsigned char);
extern void puts(char*);
extern void putsp(char*,char*);
extern void putraw(int,int,unsigned char,unsigned char);
extern void putch(unsigned char);
extern unsigned char getch();

/* /KRNL/CORE/GDT.C */
extern void gdt_install();

/* /KRNL/CORE/IDT.C */
extern void idt_set_gate(unsigned char,unsigned long,unsigned short, unsigned char);
extern void idt_install();

/* /KRNL/CORE/ISRS.C */
extern void isrs_install();

/* /KRNL/CORE/IRQ.C */
extern void irq_install_handler(int, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int);
extern void irq_install();
extern void irq_handler(struct regs *r);

/* /KRNL/CORE/PIT.C */
extern void pit_install();
extern int  get_uptime_seconds();

/* /KRNL/CORE/KB.C */
void kb_install();

/* /LIB/PRINTF.C */
extern void printf(const char*,...);
extern int strcmp(const char*,const char*);
extern void itob(int,char*,int);

/* /LIB/READLINE.C */
extern char* readline(char* prompt);

/* /CORE/CMOS.C */
extern unsigned read_cmos(unsigned);

/* /CORE/MM.C */
extern void init_paging();



#endif
