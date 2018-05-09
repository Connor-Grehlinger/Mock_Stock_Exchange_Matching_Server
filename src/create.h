#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "account.h"
#include <map>
#include <cstdio>
#include <functional>
#include <memory>
#ifndef CREATE_H
#define CREATE_H

class Create {
 private:
  //maps account ID's to Account Instances
  std::map<int, std::shared_ptr<Account> >* mymap;

 public:  
  std::map<int, std::shared_ptr<Account> > * get_map(void);

  const std::map<int, std::shared_ptr<Account> > * constget_map(void) const;

  //  void add_to_map(int& ID, int& balance);
  void add_to_map(int& ID, std::shared_ptr<Account> inst);

  Create();

  
  ~Create();
  
  //functions to insert into Account() into Position table and Account table

  friend std::ostream & operator << (std::ostream& stream, const Create& data);
  
};



#endif
