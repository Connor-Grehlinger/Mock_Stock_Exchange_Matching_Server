#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include "account.h"

Account::Account(int myID, int mybalance): ID(myID), balance(mybalance) {
  positions = new std::map<std::string, int>;
}

//getter methods
const std::map<std::string, int>* Account::get_map(void) const {
  return positions;
}

Account::~Account() {
  delete positions;
}


const int& Account::get_constID(void) const{
  return ID;
}

const int& Account::get_constbalance(void) const{
  return balance;
}

int& Account::get_ID(void){
  return ID;
}

int& Account::get_balance(void){
  return balance;
}


//setter methods
void Account::set_ID(int myID){
  ID = myID;
}

void Account::set_balance(int mybalance){
  balance = mybalance;
}

void Account::add_position(std::string pos, int value) {
  //insert pair into map
  positions->insert(std::pair<std::string, int>(pos, value));
  //  positions.emplace(pos, value);
}

void Account::change_position(std::string stock, int update) {
  //find key in map
  std::map<std::string, int>::iterator it;
  it = positions->find(stock);
  //edit value within the map
  if (it != positions->end()) {
    it->second = update;
  }
}


bool Account::has_position(std::string stock) {
  //contains method on map
  std::map<std::string, int>::iterator it;
  it = positions->find(stock);

  //if the iterator is not the end --> true
  if (it != positions->end()) {
    return true;
  }
  else {
    return false;
  }
}

int Account::pos_value(std::string stock) {
  //find the stock
  std::map<std::string, int>::iterator it;
  it = positions->find(stock);
  
  //return value in map
  return it->second;
}

/*
//pass in Order and attempt to execute
void Account::exec_order(Order& myorder); {
    //check if the stock for this order is within this Account

    //check whether this account has enough stock for a SELL (negative amount)
    //OR
    //check whether this account has enough money for a BUY (positive amount)

    //if check passes --> add to Orders Table, create opened XML
    //if fails -->create error XML
  }

*/

//stream operator
std::ostream & operator << (std::ostream & stream, const Account& data) {
  //print out Account ID
  stream<<"Account ID is:"<<data.get_constID()<<std::endl;
  //print out balance of Account
  stream<<"Account balance is:"<<data.get_constbalance()<<std::endl;
  //print out each position within the Account
  const std::map<std::string, int>* mypositions = data.get_map();
  for (std::map<std::string, int>::const_iterator it = mypositions->begin(); it != mypositions->end(); ++it) {
    stream<<"Position: "<<it->first<<" has value: "<<it->second<<std::endl;
  }
  return stream;
}
