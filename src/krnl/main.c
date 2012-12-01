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
 *  Name: main.c
 *  External: main.o
 *  Author: Andrew F. Hart
 *  Date: 2006.04.08
 * =======================================================================
 */

#include <system.h>


void *memcpy(void *dest, const void *src, size_t count)
{
  /* Add code here to copy 'count' bytes of data from 'src' to 'dest' and 
   * finally, return 'dest' */
  const char *pSource = (const char *)src;
  char *pDest = (char *)dest;
  for(; count != 0; count--) *pDest++ = *pSource++;
  return dest;
  
}

void *memset(void *dest, char val, size_t count)
{
  /* Add code here to set 'count' bytes in 'dest' to 'val' and 
   * finally, return 'dest' */
  char *temp = (char *)dest;
  for(; count != 0; count--) *temp++ = val;
  return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
  /* Same as memset but with 16-bit 'val' and dest pointer. */
  unsigned short *temp = (unsigned short *)dest;
  for(; count != 0; count--) *temp++ = val;
  return dest;
  
}

unsigned char inportb(unsigned short _port)
{
  unsigned char rv;
  /*
   * Produces the assembly instruction:
   *      inb eax, {0..255}
  */
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

void outportb(unsigned short _port, unsigned char _data)
{
  /*
   * Produces the assembly instruction:
   *      outb {0..255}, eax
   *
  */
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void itoa(int val, char* buf, int bufsize)
{
  char values[bufsize];
  int a;
  for(a=0;a<bufsize;a++){values[a]='\0';}

  int start_flag = 0;
  
  int tmpval = val;
  int i      = 0;
  int j      = 0;
  while(i < bufsize - 1)
  {
    values[i++] = (char)((tmpval % 10) + 0x30);
    tmpval /= 10;  
    if(tmpval == 0){break;}  
  } 
  /* Reverse order and place ending '\0' null terminator */

  for(i=bufsize-1,j=0;i>=0;i--)
  {
    if(values[i] >= '0'){start_flag = 1;}
    if(start_flag)
    {
      buf[j] = values[i];
      j++;
    }
  }
  buf[j] = '\0';
}

int pow(int base, int exp)
{
  int i;
  int retval = 1;
  for(i=0;i<exp;i++)
  {
    retval *= base;
  }
  return retval;
}

int atoi(char* val)
{
  int length = 0;
  int vindx  = 0;
  int nindx  = 0;


  /* Get the length of the number */
  while(val[vindx] != '\0'){length++;vindx++;}

  int retval = 0;
  for(nindx = 0; nindx < length; nindx++)
  {
    retval += ((int)(val[nindx]) - 0x30) * pow(10,length - nindx - 1);
  }

  
  return retval;  
}

void install_gdt()
{
  putsp("[krnl]: ", "Installing Global Descriptor Table...");
  gdt_install();
  puts("Done.\n");
}

void install_idt()
{
  putsp("[krnl]: ", "Installing Interrupt Descriptor Table...");
  idt_install();
  puts("Done.\n");
}

void install_isrs()
{
  putsp("[krnl]: ", "Installing Interrupt Service Routines...");
  isrs_install();
  puts("Done.\n");
}

void install_irqs()
{
  putsp("[krnl]: ", "Installing IDT Entries for Interrupt Requests...");
  irq_install();
  puts("Done.\n");
}

void install_pit()
{ 
  putsp("[krnl]: ", "Installing Programmable Interrupt Timer Handler on IRQ0...");
  pit_install();
  puts("Done.\n");
}

void welcome()
{
  puts("welcomeputs\n");
  printf("welcomeprintf\n");
  printf("\n");
  printf("        __  _________  _________  _________ /\\\n");
  printf("     __/\\_\\/\\  _____ \\/\\  _____ \\/\\   _____\\  \\\n");
  printf("    / /:/ /\\:\\ \\___/\\ \\:\\ \\___/\\ \\:\\  \\___/_   \\\n");
  printf("   / /:/ / _\\:\\ \\  \\:\\ \\:\\ \\  \\:\\ \\:\\_____  \\   \\\n");
  /*
  printf("  / /:/ / /\\ \\:\\ \\  \\:\\ \\:\\ \\  \\:\\ \\/___/:\\  \\   \\\n");
  printf("  \\ \\:\\ \\//\\\\ \\:\\ \\__\\:\\ \\:\\ \\__\\:\\ \\  _\\_:\\  \\   \\\n");
  printf("   \\ \\:\\//_/\\\\_\\:\\________\\:\\________\\/\\_______\\   \\n");   
  printf("    \\ \\:\\  \\//  \\/________/\\/________/\\/_______/   /\n");
  printf("     \\ \\:\\ //  / GOOS - GO Operating System       /\n");
  printf("      \\ \\://  /   c.2006 Andrew F. Hart          /\n");
  printf("       \\_\\/__/__________________________________/\n");
  printf("\n");
  */
}


void print_cpu(struct cpu *c)
{
  putsp("[krnl]: ","Dumping CPU Contents:\n");
  printf("EAX: %d EBX: %d ECX: %d EDX: %d\n",c->eax,c->ebx,c->ecx,c->edx);
  printf("ESP: %d EBP: %d ESI: %d EDI: %d\n",c->esp,c->ebp,c->esi,c->edi);
  printf(" CS: %d\n",c->cs);
  printf(" DS: %d\n",c->ds);
  printf(" SS: %d\n",c->ss);
  printf(" ES: %d\n",c->es);
  printf(" FS: %d\n",c->fs);
  printf(" GS: %d\n",c->gs);
  printf("EIP: %d\n",c->eip);
  putsp("[krnl]: ","CPU Dump Complete.\n"); 
}

void wait_for_enter()
{
  char c;
  while((c = getch()) != '\n');
}

void do_shutdown()
{
  cls();
  printf("It is now safe to turn off your computer.\n");
  printf("\nGoodbye!");
  /*
  printf("(or you may press enter to reboot now...\n");
  wait_for_enter();
  reboot(); 
  */ 
}


int main()
{
   /* Clear screen */
   init_cons();
   cls();

   /* Say Hello */
   welcome();
   putsp("[krnl]: ","Hello from the GOOS kernel!\n");

   /* Install the GDT */
   install_gdt();


   /* Install the IDT */
   install_idt();

   /* Install the ISRs */
   install_isrs();
   
   /* Install the IRQ IDT entries */
   install_irqs();

   /* Enable interrupts again! */
   __asm__ __volatile__("sti");
   
   /* Install the PIT */
   install_pit();  

   /* Initialize Memory Management */
   init_paging();

   for(;;);
   
   /**** Test printf and readline 
   putsp("[krnl]: ","Testing printf: ");
   printf("the %s is: %d which is %d + %d\n","answer",20+22,20,22);
   putsp("[krnl]: ","Testing readline:\n");
   char* input = readline("G> ");
   printf("\n%s\n",input);
   ******/

   /**** Test Divide by Zero exception
   int i = 2;
   while(i>=0)
   {
     putc(0x80/i);
     i--;
   }
   ******/

   /*
    * Drop into Command Interpreter to process user
    * commands. (note: ci() never returns except on shutdown)
   */
   putsp("[krnl]: ", "Starting Command Interpreter CI...\n");
   ci();

   /*
    * If getting here, something has gone wrong or user has 
    * elected to shutdown/reboot machine. 
    * Do that now.
   */
   do_shutdown();
   

   
   
   /* Infinite Loop */
   for(;;);

   /* The following just removes the compiler warning.. main()
    * never returns. (If it ever does, krnl/gkstart.asm
    * catches the return and issues a hlt instruction)
    */
   return 0;
}





