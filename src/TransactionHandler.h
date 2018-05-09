#include "ExchangeServerStorage.h"
#include "tinyxml2.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>
#include <cstdio>
#include <functional>
#include <memory>
#include "transactions.h"
#include "order.h"
#include "account.h"
#include "rapidxml.hpp"
#define ERRORNUM -92092

#ifndef TRANSACTIONHANDLER_H
#define TRANSACTIONHANDLER_H

class TransactionHandler{
private:
  ExchangeServerStorage exchangeStorage;
  //std::shared_ptr<tinyxml2::XMLDocument> mydoc;
  rapidxml::xml_document<>* mydoc;
public:
  TransactionHandler(pqxx::connection* c);

  TransactionHandler();

  ~TransactionHandler();

  rapidxml::xml_document<>* get_doc(void);
  
  std::string create_order(std::string sym, int amount, double limit, int ID);

  std::string execXML(int amount, int price,  double time);

  std::string cancelXML(int amount, int price, double time);

  std::string openXML(int amount, int price, double time);

  std::string add_status(int ID);

  std::string end_status(int ID);

  std::string add_cancel(int ID);

  std::string end_cancel(int ID);
 
  std::string error_id(int ID, int cond);

  std::string error_order(std::string sym, int amount, double limit, int cond);

  int XML_to_int(const char* val);

  double XML_to_double(const char* val);

  void createNode(rapidxml::xml_node<>* myroot,
		  std::shared_ptr<Transactions> contain,
		  std::string& response,
		  std::shared_ptr<Account> myacc,
		  int counter);

  std::string Parse_Preprocess(std::string buffer);
  
  ExchangeServerStorage& getExchangeStorage();
  // whatever methods to handle XML
 
};

#endif
