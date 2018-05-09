#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <functional>
#include <memory>
#include "order.h"
Order::Order(int account, std::string symbol, int value, double limitation): acc_ID(account), order_ID(-1), sym(symbol), amount(value), limit(limitation), closed(false), open(false), time(0){}

Order::Order(int account, int order_num, std::string symbol, int value, double limitation, double timing): acc_ID(account), order_ID(order_num), sym(symbol), amount(value), limit(limitation), closed(false), open(false),  time(timing){}

Order::Order(int account, int order_num, std::string symbol, int value, double limitation, double timing, bool _closed, bool _executed): acc_ID(account), order_ID(order_num), sym(symbol), amount(value), limit(limitation), time(timing), closed(true), open(false), canceled(canceled), executed(_executed){}



Order::~Order() {}

const int& Order::get_accID(void) const {
  return acc_ID;
}

const int& Order::get_orderID(void) const {
  return order_ID;
}

const std::string& Order::get_sym(void) const {
  return sym;
}

const int& Order::get_amount(void) const {
  return amount;
}

const double& Order::get_limit(void) const {
  return limit;
}

const double& Order::get_time(void) const {
  return time;
}


const bool Order::get_closed(void) const {
  return closed;
}

const bool Order::get_executed(void) const {
  return executed;
}

const bool Order::get_open(void) const {
  return open;
}

void Order::set_orderID(int order_num) {
  order_ID = order_num;
}

void Order::set_amount(int value) {
  amount = value;
}

void Order::set_open(bool val) {
  open = val;
}

void Order::set_closed(bool val) {
  closed  = val;
}

std::ostream & operator << (std::ostream & stream, const Order& data) {
  //print out Account ID
  stream<<"Account ID is:"<<data.get_accID()<<std::endl;
  //print out Order ID
  stream<<"Order ID is:"<<data.get_orderID()<<std::endl;

  //print out string sym
  stream<<"Symbol is:"<<data.get_sym()<<std::endl;
  
  //print out amount
  stream<<"Amount is:"<<data.get_amount()<<std::endl;

  //print out amount
  stream<<"Limit is:"<<data.get_limit()<<std::endl;  
  return stream;

}
