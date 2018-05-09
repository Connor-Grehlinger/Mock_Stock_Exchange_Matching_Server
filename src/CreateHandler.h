#include <stdlib.h>
#include "tinyxml2.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>
#include "account.h"
#include "create.h"
#include <cstdio>
#include <functional>
#include <memory>
#include "ExchangeServerStorage.h"
#include "rapidxml.hpp"
#ifndef CREATE_HANDLER_H
#define CREATE_HANDLER_H


class CreateHandler{
private:
  ExchangeServerStorage exchangeStorage;
  rapidxml::xml_document<>* mydoc;
  
public:

  CreateHandler(pqxx::connection* c);

  CreateHandler();

  ~CreateHandler();

  // whatever methods to deal with XML

  rapidxml::xml_document<>* get_doc(void);

  std::string create_acc(int ID);

  std::string create_sym(std::string sym, int ID);

  std::string error_sym(std::string sym, int ID, int cond);

  std::string error_acc(int ID, int balance, int cond);

  int XML_to_int(const char* val);

  
  void createNode(rapidxml::xml_node<>* myroot,
		  std::shared_ptr<Create> contain, std::string& response);
		  

  std::string Parse_Preprocess(std::string buffer);

  ExchangeServerStorage& getExchangeStorage();

};

#endif
