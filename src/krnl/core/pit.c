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
 *  Name: pit.c
 *  External: pit.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.17
 * =======================================================================
 */
#include <system.h>


#define GS_PIT_HZ 1

long system_ticks = 0;
int  system_seconds = 0;
char time_buf[10];

void timer_phase(int hz)
{
  int divisor = 10000; /*1193180 / hz; */		
  outportb(0x43, 0x36);
  outportb(0x40, divisor & 0xFF);
  outportb(0x40, divisor >> 8);
}

void pit_handler(struct regs *r)
{
  system_ticks++;
  
  if(system_ticks % 10 == 0)
  { 
    system_seconds++;
  }
}

void pit_install()
{
  irq_install_handler(0,pit_handler);
  timer_phase(GS_PIT_HZ); 
}

int get_uptime_seconds()
{
  return system_seconds;
}

