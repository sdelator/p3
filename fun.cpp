// Author: Sean Davis
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "funix.h"
#include "directory.h"
using namespace std;

Funix::Funix(): umask(0), time(0)
// creates currentDirectory, and sets umask and time
{
   currentDirectory = new Directory("/", umask, time, NULL);
  
}  // init()

Funix::~Funix()
{
  const char *root[] = {"cd", ".."};
  
  Directory *previousDirectory = 
          currentDirectory->cd(currentDirectory, 2, root);
  
    while(previousDirectory != currentDirectory) 
    {
       currentDirectory = previousDirectory;
       previousDirectory =  currentDirectory->cd(currentDirectory, 2, root);
    }//while
  
  delete currentDirectory;
} //funix


void Funix::cd(Funix *funix, int argCount, const char *arguments[])
// calls cd() with currentDirectory as one of its parameters
{
  currentDirectory = currentDirectory->cd(currentDirectory, 
          argCount, arguments); 
 
}  // cd())
  

int Funix::eXit(Funix *funix, int argCount, const char *arguments[])
{
  // checks "exit" command, returns 0 on proper exit
  
  if (argCount > 1)
  {
    cout << "usage: exit" << endl;
    return 1;
  }  // if more than one argument
  
  return 0;
}  // eXit()

void Funix::getCommand(Funix *funix, char *command)
//writes prompt and reads command
{
  writePrompt(funix);
  cin.getline(command, COMMAND_LENGTH);
  //command[strlen(command)] = '\0';                                                         
  time++;
}  // getCommand()



void Funix::ls(Funix *funix, int argCount, const char *arguments[])
  // calls ls() with currentDirectory as one of its parameters
{
  currentDirectory-> ls(currentDirectory, argCount, arguments);
}  // ls()


void Funix::mkdir(Funix *funix, int argCount, const char *arguments[])
  // calls mkdir() with currentDirectory as one of its parameters
{
  currentDirectory->mkdir(currentDirectory,  argCount, arguments, umask, 
    time);
}  // mkdir()


int Funix::processCommand(Funix *funix, char *command)//returns 0 on proper exit
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
      case 4: setUmask(funix, argCount, arguments); break;
      default: cout << arguments[0] << ": Command not found." << endl;
    }  // switch on commandNum   
  }  // if at least one argument
  
  return 1;
}  // processCommand()


void Funix::run(Funix *funix) 
// reads and processes commands until proper exit
{
  char command[COMMAND_LENGTH];
  Funix(funi);
  getCommand(funix, command);
  
  while (processCommand(funix, command))
    getCommand(funix, command);
}  // run()


void Funix::setUmask(Funix *funix, int argCount, const char *arguments[])
  // checks "umask" command and executes it if it is proper
{
  if (argCount != 2)
  {
    cout << "usage: umask octal" << endl;
    return;
  }  // if wrong number of arguments
  
  if (arguments[1][0] < '0' || arguments[1][0] > '7' 
      || strlen(arguments[1]) > 1)
  {
    cout << "umask: octal must be between 0 and 7" << endl;
    return;
  }  // if incorrect octal
 
  umask = atoi(arguments[1]);
}  // umask()



void Funix::writePrompt(Funix *funix)  // shows path and '#'
{
  currentDirectory->showPath(currentDirectory);
  cout << " # ";
}  // writePrompt()

