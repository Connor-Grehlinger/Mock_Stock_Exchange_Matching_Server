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
#include "rapidxml.hpp"
#include "ExchangeServerStorage.h"
#include "CreateHandler.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
CreateHandler::CreateHandler(pqxx::connection* c): exchangeStorage(c){
  mydoc = new rapidxml::xml_document<>;
  //  mydoc(new rapidxml::xml_document<>*);
}


CreateHandler::CreateHandler():exchangeStorage(NULL) {
  mydoc = new rapidxml::xml_document<>();
  //mydoc(new rapidxml::xml_document<>*);
}

CreateHandler::~CreateHandler(){}

  // whatever methods to deal with XML

ExchangeServerStorage& CreateHandler::getExchangeStorage(){
  return this->exchangeStorage;
}

rapidxml::xml_document<>* CreateHandler::get_doc(void) {
  return mydoc;
}


//create XML for account creation
std::string CreateHandler::create_acc(int ID) {
  std::stringstream mystring;
  mystring<<"\t<created id= "<<ID<<"\\>"<<"\n";
  return mystring.str();
}



//creates XML for symbol creation
std::string CreateHandler::create_sym(std::string sym, int ID) {
  std::stringstream mystring;
  mystring<<"\t<created sym= "<<sym<<" id= "<<ID<<"/>"<<"\n";
  return mystring.str();
}

std::string CreateHandler::error_sym(std::string sym, int ID, int cond) {
 std::stringstream mystring;

 //Account does not exist for 
 if (cond == 1) {
   mystring<<"\t<error sym= "<<sym<<" id= "<<ID<<"/>"<<"account does not exist"<<"</error>\n";
 }
 
  return mystring.str();
}


//creates error XML depending on condition
std::string CreateHandler::error_acc(int ID, int balance, int cond) {
  std::stringstream mystring;

  //cond is 1 --> missing ID
  if (cond == 1) {
    mystring<<"\t<error>"<<"No account ID"<<"</error>\n";
   }

  //cond is 2 --> missing balance
  else if (cond == 2) {
    mystring<<"\t<error "<<"id="<<ID<<">"<<"No balance"<<"</error>\n";
   }
  
  //cond is 3 --> account already exists
  else if (cond == 3) {
    mystring<<"\t<error "<<"id="<<ID<<">"<<"Account already exists"<<"</error>\n";
  }
  //cond is 4 --> no account or balance filled
  else if (cond == 4) {
    mystring<<"\t<error "<<">"<<"No account or balance"<<"</error>\n";
  }
  //cond is 5 --> not account or balance
  else if (cond == 5) {
    mystring<<"\t<error "<<">"<<"Malformed request"<<"</error>\n";
  }  
  
  return mystring.str();
}


//gets int from const char * and checks for ERRORS 
int CreateHandler::XML_to_int(const char* val) {
  std::stringstream strVal;
  int number;
  strVal << val;
  //Error checking
  if (strVal.str().empty()) {
    return -1;
  }
  else {
    strVal >> number;
    return number;
  }
}



//reads XML --> makes Account instances 
void CreateHandler::createNode(rapidxml::xml_node<>* myroot,
		std::shared_ptr<Create> contain, std::string& response) {
  //what we compare our attributes to determine if they are symbols or accounts
  char acc[] = "account";
  char sym[] = "symbol";
  int ID = -1;
  int balance = -1;
  int num;
  const char * parent;
  std::map<int, std::shared_ptr<Account> >* mymap = contain.get()->get_map();
  //Base Case for Recursion: exit function
  if (myroot == NULL) {    
    return;
  }

  
  //ATTRIBUTE IS ACCOUNT
  if ((strcmp(myroot->name(), acc) == 0) && (strcmp(myroot->parent()->name(), sym) != 0)) {
    //Save ID
    //std::cout<<"Inside account if tree"<<std::endl;
    //std::cout<<"This node's name is:"<<myroot->name()<<std::endl;
    const char * val = myroot->first_attribute()->value(); // failling here
    //std::cout<<"My ID is:"<<val<<std::endl;
    ID = XML_to_int(val);
    //Save balance
    const char * bal = myroot->last_attribute()->value();
    //std::cout<<"My balance is:"<<bal<<std::endl;
    balance = XML_to_int(bal);
    //create an Account if there are No Errors
    if ((balance !=-1)&&(ID != -1)) {
      std::shared_ptr<Account> newacc(new Account(ID, balance));
      // mymap->emplace(ID, newacc);

      //attempt to insert
      bool insertResult = this->exchangeStorage.insertAccount(newacc);
      if (insertResult){
	//if success: append create success
	//std::cout<<"Our insert of an account worked!"<<std::endl;
	response.append(create_acc(ID));
      }
      else{
	response.append(error_acc(ID, balance, 3));
      }

      //if fail: append error message --> response.append(err_acc(ID, balance, 3)
     }
    //No  ID
    else if ((ID == -1)&&(balance!=-1)) {
      response.append(error_acc(ID, balance, 1));
    }
    else if ((balance == -1)&&(ID != -1)) {
      //if parent is NOT SYMBOL --> error
      parent = myroot->parent()->name();
      if (strcmp(parent, sym) != 0) {
	response.append(error_acc(ID, balance, 2)); 
      }
    }
    else {
      response.append(error_acc(ID, balance, 4));
    }
  }
  
  parent = myroot->parent()->name();
  if (parent != NULL) {
    //if our parent is SYM --> We have to add a SYM to an Existing Account
    if (strcmp(parent, sym) == 0) {
      //std::cout<<"My parent is SYM!"<<std::endl;
      const char * text = myroot->value();
      //std::cout<<"The amount of stock is :"<<text<<std::endl;
      const char * stock = myroot->parent()->first_attribute()->value();
      std::string mystock(stock);
      //std::cout<<"The stock is:"<<mystock<<std::endl;
      const char * checkID = myroot->first_attribute()->value();
      //std::cout<<"The ID is:"<<checkID<<std::endl;
      num = XML_to_int(text);
      ID = XML_to_int(checkID);
	//first find the ID
	//std::map<int, std::shared_ptr<Account> >::iterator it = mymap->find(ID);

	std::shared_ptr<Account> location = exchangeStorage.findAccount(ID);
	//If nothing found: Error
	if (location.get() == NULL) {
	  //std::cout<<"Did not find anything!"<<std::endl;
	  response.append(error_sym(mystock, ID, 1));
	}
	else {
	  //mymap->find(ID)->second->add_position(mystock, num);
	  //std::cout<<"This is the Account info I found:"<<std::endl;
	  //std::cout<<*(location.get())<<std::endl;
	  //INSERT SYMBOL FUNCTION
	  bool posinsert = exchangeStorage.insertPosition(location, mystock, num);
	  if (posinsert) {
	    //std::cout<<"We inserted a SYMBOL!"<<std::endl;
	    response.append(create_sym(mystock, ID));
	  }
	}
    }
  }
  //check for malformed request
  if ((strcmp(myroot->name(), acc) != 0) && strcmp(myroot->name(), sym) != 0) {
    //std::cout<<"The name of my node that is malformed for CreateHandler is:"<<myroot->name()<<std::endl;
    response.append(error_acc(ID, balance, 5));
  }

  //Recurse to children
  if (myroot->first_node()!= NULL) {
    //char * c = myroot->first_node()->name();
    //if (c[0] != '\0') {
    //std::cout<<"I'm recursing on my children!"<<std::endl;
    //std::cout<<"MY child is:"<<myroot->first_node()->name()<<std::endl;
      char *c = myroot->first_node()->name();
      if (c[0] != '\0') {
	//	std::cout<<"We are NULL do not recurse!"<<std::endl;
	createNode(myroot->first_node(), contain, response);
      }
  }

  //Recurse to Siblings if they exist
  if (myroot->next_sibling() != NULL) {
    //har * d = myroot->first_node()->name();
    //std::cout<<"I'm recursing on my siblings!"<<std::endl;
    //std::cout<<"MY sibling is:"<<myroot->next_sibling()->name()<<std::endl;
    char *d = myroot->next_sibling()->name();
    if (d[0] != '\0') {
      //std::cout<<"We are NULL do not recurse!"<<std::endl;
      createNode(myroot->next_sibling(), contain, response);
    }
  }
}



std::string CreateHandler::Parse_Preprocess(std::string buffer) {

  //response XML
  std::string response = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  response.append("<results>\n");

  //std::cout<<"The original buffer is:"<<buffer<<std::endl;
  
  //clear first line
  buffer.erase(0, buffer.find("\n")+1);
  std::shared_ptr<Create> mycreate (new Create());

  char* cstr = new char[buffer.size() + 1];  // Create char buffer to store string copy
  strcpy (cstr, buffer.c_str());             // Copy string into char buffer
  mydoc->parse<0>(cstr);  
  
  
  createNode(mydoc->first_node()->first_node(), mycreate, response);
  response.append("<\\results>\n");
  //std::cout<<"My response now is:\n"<<response;
  //std::cout<<"Print my Map!"<<std::endl;
  //std::cout<<*(mycreate.get());
  return response;
}
