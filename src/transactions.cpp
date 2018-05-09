#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "order.h"
#include <map>
#include <cstdio>
#include <functional>
#include "transactions.h"
#include <memory>

std::map<int, std::shared_ptr<Order> >* Transactions::get_map(void) {
  return mymap;

}

const std::map<int, std::shared_ptr<Order> >* Transactions::constget_map(void) const {
  return mymap;
}

Transactions::Transactions() {
  mymap = new std::map<int, std::shared_ptr<Order> >;
}

Transactions::~Transactions() {
  delete mymap;
}


void Transactions::add_to_map(int& orderID, std::shared_ptr<Order> inst) {

  mymap->insert(std::pair<int&, std::shared_ptr<Order> >(orderID, inst));
}

std::ostream & operator << (std::ostream & stream, const Transactions& data) {
  //print out Transactions in map
  const std::map<int, std::shared_ptr<Order> >* map = data.constget_map();
  for (std::map<int, std::shared_ptr<Order> >::const_iterator it = map->begin(); it != map->end(); ++it) {
    stream<<*(it->second)<<std::endl;
  }
  return stream;
}
