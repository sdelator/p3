// Author: Sean Davis

#include <iostream>
#include "permissions.h"
using namespace std;

void Permission::set(Permissions permissions, short originalPermissions, short umask)
{
  permissions = originalPermissions & ~umask;
}  // set()


void Permissions::print(const Permissions permissions)const
{
  if (permissions & 4)
    cout<<"r";
  else  // no read permissions
    cout<<"-";
  
  if (permissions & 2)
    cout<<"w";
  else  // no write permissions
    cout<<"-";
  
  if (permissions & 1)
    cout<<"x";
  else  // no execute permissions
    cout<<"-";
}  // print()
