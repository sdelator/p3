// Author: Sean Davis
#include <cstring>
#include <iostream>
#include <cstdlib>
#include "directory.h"

using namespace std; 

Directory::Directory(const char *nam, short umask, int tim, 
                     Directory *paren):time(tim), subDirectories(NULL), 
                     subDirectoryCount(0), parent(paren)
{
   name = new char[strlen(nam) + 1];
   strcpy(name , nam);
   permissions.set(7, umask);
}  // createDirectory())

Directory::~Directory()
{
  
  delete name;
  
  for (int i = 0 ; i <  subDirectoryCount ; i++)
  {
    delete subDirectories[i];
  }//for
  
  delete [] subDirectories;
}//dir

Directory* Directory::cd(Directory *directory, int argCount, 
                         const char *arguments[])
{
  if (argCount != 2)
  {
    cout << "usage: cd directoryName" << endl;
    return this;
  }  // if two many arguments

  if (strcmp(arguments[1], "..") == 0)
  {
    if (parent)
      return parent;
    else  // this is root
      return this;
  }  // if cd ..

  for (int i = 0; i < subDirectoryCount; i++)
  {
    if (strcmp(subDirectories[i]->name, arguments[1]) == 0)
      return subDirectories[i];
  }  // for each subdirectory
 
  cout << "cd:" << arguments[1] << " No such file or directory" << endl;
  return this;
}  // cd()



void Directory::ls(const Directory *directory, int argCount, 
                   const char *arguments[])
{
  if (argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l") != 0))
    cout << "usage: ls [-l]" << endl;
  else  // correct number of arguments
  {
    if (argCount == 1)  // simple ls
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)                
        cout << subDirectories[i]->name << " ";
      
      cout << endl;
    }  // if simple ls
    else  // must be ls -l
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)
      {
        permissions.print();
        cout << " " << subDirectories[i]->time << " " 
          << subDirectories[i]->name << endl;
      }  // for each subdirectory
    }  // else is ls -l
  }  // else correct arguments
}  // ls()


void Directory::mkdir(Directory *directory,  int argCount, 
                      const char *arguments[], short umask, int time)
{
  if (argCount != 2)
  {
    cout << "usage: mkdir directory_name" << endl;
    return;
  }  // if too many arguments
  
  if (subDirectoryCount == MAX_DIRECTORIES)
  {
    cout << "mkdir: " << name <<  
            " already contains the maximum number of directories" << endl;
    return;
  }  // if already at max subdirectories
  
  for (int i = 0; i < subDirectoryCount; i++)
  {
    if (strcmp(subDirectories[i]->name, arguments[1]) == 0)
    {
      cout << "mkdir: cannot create directory '" 
              << arguments[1] << "': File exists" << endl;
      return;
    }  // if subdirectory already exists.
  }  // for each subdirectory
  
  Directory* *subDirectoriesTemp = 
    new Directory*[(subDirectoryCount + 1)];
    
  for (int i = 0; i < subDirectoryCount; i++)
    subDirectoriesTemp[i] = subDirectories[i];
  
  delete [] subDirectories;
  subDirectories = subDirectoriesTemp;
  subDirectories[subDirectoryCount] = new Directory(arguments[1], umask, 
          time, directory);
  subDirectoryCount++; 
}  // mkdir()


void Directory::showPath(const Directory *directory)
{
  if (directory->parent == NULL)
  {
    cout << "/";
    return;
  }  // at root
  
  showPath(directory->parent);
  cout << directory->name << "/";
}  // showPath())
