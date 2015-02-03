// Author: Sean Davis
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "funix.h"
#include "directory.h"

void cd(Funix *funix, int argCount, const char *arguments[])
// calls cd() with currentDirectory as one of its parameters
{
  funix->currentDirectory = cd(funix->currentDirectory, argCount, arguments); 
}  // cd())
  

int eXit(Funix *funix, int argCount, const char *arguments[])
{
  // checks "exit" command, returns 0 on proper exit
  
  if (argCount > 1)
  {
    cout<<"usage: exit"<<endl;
    return 1;
  }  // if more than one argument
  
  return 0;
}  // eXit()

void getCommand(Funix *funix, char *command)  // writes prompt and reads command
{
  writePrompt(funix);
  fgets(command, COMMAND_LENGTH, stdin);
  command[strlen(command) - 1] = '\0';
  funix->time++;
}  // getCommand()


void init(Funix *funix)  // creates currentDirectory, and sets umask and time
{
  funix->time = 0;
  funix->umask = 0;
  funix->currentDirectory = new Directory*("/",funix->umask,funix->time,NULL);
  
}  // init()


void ls(Funix *funix, int argCount, const char *arguments[])
  // calls ls() with currentDirectory as one of its parameters
{
  ls(funix->currentDirectory, argCount, arguments);
}  // ls()


void mkdir(Funix *funix, int argCount, const char *arguments[])
  // calls mkdir() with currentDirectory as one of its parameters
{
  mkdir(funix->currentDirectory,  argCount, arguments, funix->umask, 
    funix->time);
}  // mkdir()


int processCommand(Funix *funix, char *command)  // returns 0 on proper exit
{
  static const char *commands[] = {"cd", "exit", "ls", "mkdir", "umask"};
  const char *arguments[MAX_ARGUMENTS];
  char *ptr;
  int argCount = 0, commandNum;
  ptr = strtok(command, " ");
  
  while (ptr)
  {
    arguments[argCount++] = ptr;
    ptr = strtok(NULL, " ");
  }  // while more arguments in the command line
  
  if (argCount > 0)
  {
    for (commandNum = 0; commandNum < NUM_COMMANDS; commandNum++)
    {
      if (strcmp(arguments[0], commands[commandNum]) == 0)
        break;
    }  // for each possible command

    switch (commandNum)
    {
      case 0: cd(funix, argCount, arguments); break;
      case 1: return eXit(funix, argCount, arguments);
      case 2: ls(funix, argCount, arguments); break;
      case 3: mkdir(funix, argCount, arguments); break;
      case 4: umask(funix, argCount, arguments); break;
      default: cout<<arguments[0]<<"Command not found."<<endl;
    }  // switch on commandNum   
  }  // if at least one argument
  
  return 1;
}  // processCommand()


void run(Funix *funix) 
// reads and processes commands until proper exit
{
  char command[COMMAND_LENGTH];
  init(funix);
  getCommand(funix, command);
  
  while (processCommand(funix, command))
    getCommand(funix, command);
}  // run()


void setUmask(Funix *funix, int argCount, const char *arguments[])
  // checks "umask" command and executes it if it is proper
{
  if (argCount != 2)
  {
    cout<<"usage: umask octal"<<endl;
    return;
  }  // if wrong number of arguments
  
  if (arguments[1][0] < '0' || arguments[1][0] > '7' 
      || strlen(arguments[1]) > 1)
  {
    cout<<"umask: octal must be between 0 and 7"<<endl;
    return;
  }  // if incorrect octal
  
  funix->umask = atoi(arguments[1]);
}  // umask()



void writePrompt(Funix *funix)  // shows path and '#'
{
  showPath(funix->currentDirectory);
  cout<<" # ";
}  // writePrompt()

