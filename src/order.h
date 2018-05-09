#include "tinyxml2.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
 #include <cstdio>
#include <functional>
#include <memory>
#ifndef ORDER_H
#define ORDER_H

class Order {
 private:
  int acc_ID;
  int order_ID;
  std::string sym;
  int amount;
  double limit;
  bool closed;
  bool open;
  bool canceled;
  bool executed;
  double time;

 public:

  Order(int account, std::string symbol, int value, double limitation);

  Order(int account, int order_num, std::string symbol, int value, double limitation,
	double timing);

  Order(int account, int order_num, std::string symbol, int value, double limitation,
	double timing, bool _closed, bool _executed);

  
  ~Order();

  const int& get_accID(void) const;

  const int& get_orderID(void) const;

  const std::string& get_sym(void) const;

  const int& get_amount(void) const;

  const double& get_limit(void) const;

  const double& get_time(void) const;

  const bool get_closed(void) const;

  const bool get_executed(void) const;

  const bool get_open(void) const;
  
  void set_open(bool val);

  void set_closed(bool val);
  
  void set_orderID(int order_num);

  void set_amount(int value);

  friend std::ostream & operator << (std::ostream& stream, const Order& data);

};

#endif
