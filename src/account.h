#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
//#include "order.h"

class Account{

 private:
  //acount ID
  int ID;
  //balance
  int balance;
  //holds positions of account
  std::map<std::string, int> * positions;
  
 public:
  
  Account(int myID, int mybalance);

  ~Account();
  
  const std::map<std::string, int>* get_map(void) const;

  const int& get_constID(void) const;

  const int& get_constbalance(void) const;

  int& get_ID(void);

  int& get_balance(void);

  
  //edits the value of a position
  void change_position(std::string stock, int update);

  void set_ID(int myID);
 
  void set_balance(int mybalance);

  void add_position(std::string pos, int value);
  
  //tells if you a stock is there or not
  bool has_position(std::string stock);

  //gets value of a position
  int pos_value(std::string stock);

  /*
  //pass in Order and attempt to execute
  void exec_order(Order& myorder); {
    //check if the stock is there

    //check whether this account has enough stock for a SELL (negative amount)
    //OR
    //check whether this account has enough money for a BUY (positive amount)

    //if check passes --> add to Orders Table, create opened XML
    //if fails -->create error XML
  }
  */
  //overloaded operator to print information of this account
  friend std::ostream & operator << (std::ostream & stream, const Account & data);
  
};


#endif
