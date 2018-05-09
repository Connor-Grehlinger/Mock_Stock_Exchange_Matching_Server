#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "order.h"
#include <map>
#include <cstdio>
#include <functional>
#include <memory>
#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

class Transactions {
 private:
  //maps account ID's to Order Instances
  std::map<int, std::shared_ptr<Order> >* mymap;

 public:
  
  Transactions();

  std::map<int, std::shared_ptr<Order> > * get_map(void);

  const std::map<int, std::shared_ptr<Order> > * constget_map(void) const;

  void add_to_map(int& ID, std::shared_ptr<Order> inst);  

  ~Transactions();
  
  friend std::ostream & operator << (std::ostream& stream, const Transactions& data);
  
};



#endif
