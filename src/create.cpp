#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "account.h"
#include "create.h"
#include <map>
#include <sstream>
#include <cstdio>
#include <functional>
#include <memory>

std::map<int, std::shared_ptr<Account> >* Create::get_map(void) {
  return mymap;

}

const std::map<int, std::shared_ptr<Account> >* Create::constget_map(void) const {
  return mymap;
}

Create::Create() {
  mymap = new std::map<int, std::shared_ptr<Account> >;
}

Create::~Create() {
  delete mymap;
}


void Create::add_to_map(int& ID, std::shared_ptr<Account> inst) {

  mymap->insert(std::pair<int&, std::shared_ptr<Account> >(ID, inst));
}

std::ostream & operator << (std::ostream & stream, const Create& data) {
  //print out Accounts in map
  const std::map<int, std::shared_ptr<Account> >* map = data.constget_map();
  for (std::map<int, std::shared_ptr<Account> >::const_iterator it = map->begin(); it != map->end(); ++it) {
    stream<<*(it->second);
  }
  return stream;
}
