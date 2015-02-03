// Author: Sean Davis

#ifndef PERMISSIONS_H
  #define PERMISSIONS_H

class Permissions
{
  short permissions;
  public: 

  void set(Permissions permissions, short originalPermissions, 
                       short umask);
  void print(const Permissions permissions)const;
}; //class Permissions
#endif  // PERMISSIONS_H

