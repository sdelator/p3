// Author: Sean Davis
#include <cstring>
#include <iostream>
#include <cstdlib>
#include "directory.h"
using namespace std; 

Directory::Directory(const char *nam, short umask, 
                     int tim, Directory *paren):time(tim), subDirectories(NULL)\
                     , subDirectoryCount(0), parent(paren)
{
  name = new char[strlen(nam) + 1];
  set(permissions, 7, umask);
}  // createDirectory())

Directory* Directory::cd(Directory *directory, int argCount, const char *arguments[])
{
  if (argCount != 2)
  {
    cout<<"usage: cd directoryName"<<endl;
    return directory;
  }  // if two many arguments

  if (strcmp(arguments[1], "..") == 0)
  {
    if (parent)
      return parent;
    else  // this is root
      return directory;
  }  // if cd ..

  for (int i = 0; i < subDirectoryCount; i++)
  {
    if (strcmp(subDirectories[i]->name, arguments[1]) == 0)
      return subDirectories[i];
  }  // for each subdirectory
 
  cout<<"cd:" <<arguments[1]<< "No such file or directory"<<endl;
  return directory;
}  // cd()



void Directory::ls(const Directory *directory, int argCount, const char *arguments[])
{
  if (argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l") != 0))
    cout<<"usage: ls [-l]"<<endl;
  else  // correct number of arguments
  {
    if (argCount == 1)  // simple ls
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)
        cout<<name<<subDirectories[i]->name<<endl;
      
    }  // if simple ls
    else  // must be ls -l
    {
      for (int i = 0; i < directory->subDirectoryCount; i++)
      {
        print(subDirectories[i]->permissions);
        cout<<time<<name<<subDirectories[i]->time<< 
          subDirectories[i]->name<<endl;
      }  // for each subdirectory
    }  // else is ls -l
  }  // else correct arguments
}  // ls()


void Directory::mkdir(Directory *directory,  int argCount, const char *arguments[], 
           short umask, int time)
{
  if (argCount != 2)
  {
    cout<<"usage: mkdir directory_name"<<endl;
    return;
  }  // if too many arguments
  
  if (subDirectoryCount == MAX_DIRECTORIES)
  {
    cout<<"mkdir: "<<name<< "already contains the maximum number of directories"
            <<endl;
    return;
  }  // if already at max subdirectories
  
  for (int i = 0; i < subDirectoryCount; i++)
  {
    if (strcmp(subDirectories[i]->name, arguments[1]) == 0)
    {
      cout<<"mkdir: cannot create directory "<<arguments[1]<<": File exists" 
        <<endl;
      return;
    }  // if subdirectory already exists.
  }  // for each subdirectory
  
  Directory* *subDirectoriesTemp = 
    new Directory*[sizeof(Directory) * (subDirectoryCount + 1)];
    
  for (int i = 0; i < subDirectoryCount; i++)
    subDirectoriesTemp[i] = subDirectories[i];
  
  delete [] subDirectories;
  subDirectories = subDirectoriesTemp;
  Directory(arguments[1], umask, 
                     time, directory);
  
  directory->subDirectoryCount++; 
}  // mkdir()


void Directory::showPath(const Directory *directory)
{
  if (directory->parent == NULL)
  {
    cout<<"/";
    return;
  }  // at root
  
  showPath(parent);
  cout<<name<<"/";
}  // showPath())
