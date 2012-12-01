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
 *  Name: ci.h
 *  External: CI.H
 *  Author: Andrew F. Hart
 *  Date: 2006.05.27
 * =======================================================================
 */

void ci_clear(int argc,char **argv);
void ci_cregs(int argc,char **argv);
void ci_echo(int argc,char **argv);
void ci_help(int argc,char **argv);
void ci_uptime(int argc,char **argv);

int next_arg(char**,char**,int);

struct ci_command
{
  const char* name;
  const char* desc;
  void (*func)(int argc,char **argv);
};

/* 
 * CI Command Definitions 
 *
*/
static struct ci_command ci_commands[] = 
{ 
  {"clear","Clear the screen",ci_clear},
  {"cls","Alias of \"Clear\"",ci_clear},
  {"cregs","Display Control Register Values",ci_cregs},
  {"echo","Print a message to the terminal",ci_echo},
  {"help","Display this message",ci_help},
  {"uptime","Display system uptime stats",ci_uptime},
};


static int ci_num_commands = sizeof(ci_commands) / sizeof(ci_commands[0]);
#define MAX_ARGS 16
