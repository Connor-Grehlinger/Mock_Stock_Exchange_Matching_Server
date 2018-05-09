#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <pqxx/except.hxx>
#include "order.h"
#include "account.h"

#ifndef EXCHANGESERVERSTORAGE_H
#define EXCHANGESERVERSTORAGE_H

class ExchangeServerStorage{
private:
  pqxx::connection * exchangeDBConnection;
  
public:

  ExchangeServerStorage(pqxx::connection* c);

  ~ExchangeServerStorage();

  // good
  void printAccounts();

  // need to implement
  bool insertAccount(std::shared_ptr<Account>& account);

  bool insertPosition(std::shared_ptr<Account>& account, std::string sym,
		      int value);

  bool insertOrder(std::shared_ptr<Order>& order);
  
  // good 
  void printPositions();

  void printOpenOrder();
  
  //find Account
  std::shared_ptr<Account> findAccount(int ID);

  //finds Orders
  std::shared_ptr<Account> findOrder(int ID);

  std::shared_ptr<Order> findClosedOrder(int ID);
  
  std::map<int,std::shared_ptr<Order> > Buy_List();

  std::map<int,std::shared_ptr<Order> > Sell_List();

  std::list<std::shared_ptr<Order> > queryOrdersByID(int ID);
};

#endif
