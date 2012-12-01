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
 *  Name: ci.c
 *  External: ci.o
 *  Author: Andrew F. Hart
 *  Date: 2006.05.27
 * =======================================================================
 */
#include <system.h>
#include <ci.h>

/*
 * CI - Command Interpreter
 *
 * The kernel drops into CI when it has finished setting
 * up the environment. CI never returns. It tries to match
 * user input to a list of known commands, and, if a match
 * is found, carries out the command.
 * 
*/

void ci()
{
  printf("CI> Welcome to the GOOS command line interpreter CI.\n");
  printf("CI> Type \"help\" to see a list of available commands.\n");
  char *command;
  for(;;)
  {

    int argc = 0;
    char *argv[MAX_ARGS];
    int error=0;

    /* Get command from user */
    command = readline("CI> ");
    if(strcmp(command,"") == 0){continue;}
    int next= 0;

    /* Parse Command string into command and arguments */ 
    do
    {
      if(argc == MAX_ARGS)
      {
        printf("Error: Too many arguments (max is %d).\n",MAX_ARGS);
        error = 1;
        break;        
      }
      argv[argc] = 0;
      next = next_arg(&command,&(argv[argc]),next);
      argc++;
    } while(next);
    
    /* If argument overflow, restart */
    if(error){continue;} 
    
    /* If user types exit, quit CI by returning to main()
     * (/krnl/main.c) 
    */
    if(strcmp(argv[0],"exit") == 0){return;}   
    
    /* Look up and invoke command */
    int i;
    for(i=0;i<ci_num_commands;i++)
    {
      if(strcmp(ci_commands[i].name,argv[0]) == 0)
      {
        ci_commands[i].func(argc,argv);
        break;
      }
    }
    /* If command not found, inform user and continue */
    if(i>= ci_num_commands)
    {
      printf("command not found. (got: %s)\n",argv[0]);   
    }

  }
}


int next_arg(char **cmdstr,char **argbuf,int start)
{
  int i = start;
  int charflag = 0; 	/* have we seen a character yet? */
  
  for(;;)
  {
    /* If end of string reached, return */
    if((*cmdstr)[i] == '\0'){(*cmdstr)[i] = '\0'; return (0);}
    /* Consume excess whitespace (before first char) */
    if((*cmdstr)[i] == ' ' && !charflag){i++;}
    /* Consume excess characters (after first char)*/
    if((*cmdstr)[i] > ' ' && charflag){i++;}
    /* On encountering first character, save the pointer
    */
    if((*cmdstr)[i] > ' ' && !charflag)
    { 
      charflag = 1;
      *argbuf = *cmdstr + i;
    }
    /* scan past argument until find a space, 
     * replace space with null terminator and
     * return the index of the next character
    */
    if((*cmdstr)[i] == ' ' && charflag)
    {
      (*cmdstr)[i] = '\0';
      return (++i);
    }
  }
}

/*
 * CI Command Implementations
 *
*/

/* CLEAR
*/
void ci_clear(int argc,char **argv)
{
  cls();
  
}
/* CLS (aliased to CLEAR in /incl/ci.h)
*/

/* CREGS
*/
void ci_cregs(int argc, char **argv)
{
   int cr0 = read_cr0();
   int cr1 = read_cr1();
   int cr2 = read_cr2();
   int cr3 = read_cr3();
   printf("Control Register Values\n");
   printf("\tcr0: %d\n",cr0);
   printf("\tcr1: %d\n",cr1);
   printf("\tcr2: %d\n",cr2);
   printf("\tcr3: %d\n",cr3);
}

/* ECHO
*/
void ci_echo(int argc, char **argv)
{
  if(argc < 2)
  {
    printf("Not enough arguments.\n");
    printf("Usage: ECHO arg\n");
    return;
  }
  printf("%s\n",(argv)[1]);
}

/* HELP
*/
void ci_help(int argc, char **argv)
{
  printf("CI - Command List \n");
  printf("----------------------------------------\n");
  printf("exit\tExit GOOS (shutdown / reboot)\n");
  int i;
  for(i=0;i< ci_num_commands; i++)
  {
    printf("%s\t%s\n",ci_commands[i].name,ci_commands[i].desc);
  }
}

/* UPTIME
*/
void ci_uptime(int argc, char **argv)
{
  printf("The system has been running for %d seconds\n",get_uptime_seconds());
  printf("The time is: %d:%d:%d\n",read_cmos(0x4),read_cmos(0x2),read_cmos(0x0));
  printf("The day of the week is: %d\n",read_cmos(0x6));
  printf("The date of the month is: %d\n",read_cmos(0x7));
  printf("The month is: %d\n",read_cmos(0x8));
  printf("The year is: %d (%b)\n",read_cmos(0x9),read_cmos(0x9));
  printf("test: 32 is: %b, 63 is: %b, 65 is %b\n",32,63,65);
  printf("tes2: 5097 is: %b, 1000000 is: %b\n",5097,1000000);
  printf("CMOS Control Reg B: %b\n",read_cmos(0xb));


}

