// Author: Sean Davis
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "directory.h"


Directory* cd(Directory *directory, int argCount, const char *arguments[])
{
  if (argCount != 2)
  {
    printf("usage: cd directoryName\n");
    return directory;
  }  // if two many arguments

  if (strcmp(arguments[1], "..") == 0)
  {
    if (directory->parent)
      return directory->parent;
    else  // this is root
      return directory;
  }  // if cd ..

  for (int i = 0; i < directory->subDirectoryCount; i++)
  {
    if (strcmp(directory->subDirectories[i].name, arguments[1]) == 0)
      return &directory->subDirectories[i];
  }  // for each subdirectory
 
  printf("cd: %s: No such file or directory\n", arguments[1]);
  return directory;
}  // cd()

void createDirectory(Directory *directory, const char *name, short umask, 
                     int time, Directory *parent)
{
  directory->name = (char*) malloc(strlen(name) + 1);
  directory->subDirectoryCount = 0;
  directory->subDirectories = NULL;
  strcpy(directory->name, name);
  createPermissions(&directory->permissions, 7, umask);
  directory->parent = parent;
  directory->time = time;
}  // createDirectory())


void ls(const Directory *directory, int argCount, const char *arguments[])
{
  if (argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l") != 0))
    printf("usage: ls [-l]\n");
  else  // correct number of arguments
  {
    if (argCount == 1)  // simple ls
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)
        printf("%s ", directory->subDirectories[i].name);
      
      printf("\n");
    }  // if simple ls
    else  // must be ls -l
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)
      {
        printPermissions(&directory->subDirectories[i].permissions);
        printf(" %d %s\n", directory->subDirectories[i].time, 
          directory->subDirectories[i].name);
      }  // for each subdirectory
    }  // else is ls -l
  }  // else correct arguments
}  // ls()


void mkdir(Directory *directory,  int argCount, const char *arguments[], 
           short umask, int time)
{
  if (argCount != 2)
  {
    printf("usage: mkdir directory_name\n");
    return;
  }  // if too many arguments
  
  if (directory->subDirectoryCount == MAX_DIRECTORIES)
  {
    printf("mkdir: %s already contains the maximum number of directories\n",
      directory->name);
    return;
  }  // if already at max subdirectories
  
  for (int i = 0; i < directory->subDirectoryCount; i++)
  {
    if (strcmp(directory->subDirectories[i].name, arguments[1]) == 0)
    {
      printf("mkdir: cannot create directory '%s': File exists\n", 
        arguments[1]);
      return;
    }  // if subdirectory already exists.
  }  // for each subdirectory
  
  Directory *subDirectoriesTemp = (Directory*) 
    malloc(sizeof(Directory) * (directory->subDirectoryCount + 1));
    
  for (int i = 0; i < directory->subDirectoryCount; i++)
    subDirectoriesTemp[i] = directory->subDirectories[i];
  
  free(directory->subDirectories);
  directory->subDirectories = subDirectoriesTemp;
  createDirectory(&directory->subDirectories[directory->subDirectoryCount], 
    arguments[1], umask, time, directory);
  directory->subDirectoryCount++; 
}  // mkdir()


void showPath(const Directory *directory)
{
  if (directory->parent == NULL)
  {
    printf("/");
    return;
  }  // at root
  
  showPath(directory->parent);
  printf("%s/", directory->name);
}  // showPath())
