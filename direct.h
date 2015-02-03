// Author: Sean Davis

#ifndef DIRECTORY_H
  #define DIRECTORY_H

#include "permissions.h"

const int MAX_DIRECTORIES 3

class Directory
{
  char *name;
  int time;
  Directory**subDirectories;
  int subDirectoryCount;
  Directory* parent;
  Permissions permissions;
public:
Directory(const char *nam, short umask, 
                     int tim, Directory *paren);
Directory* cd(Directory *directory, int argCount, const char *arguments[]);

void ls(const Directory *directory, int argCount, const char *arguments[]);
void mkdir(Directory *directory,  int argCount, const char *arguments[], 
           short umask, int time);
void showPath(const Directory *directory);
};
#endif  // DIRECTORY_H

