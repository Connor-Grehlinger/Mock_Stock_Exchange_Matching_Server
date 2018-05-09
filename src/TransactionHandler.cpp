#include "TransactionHandler.h"
#include "tinyxml2.h"
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
#include "ExchangeServerStorage.h"
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>
#include <cstdio>
#include <functional>
#include <memory>
#include <list>
#include "transactions.h"

TransactionHandler::TransactionHandler(pqxx::connection * c):exchangeStorage(c){
  mydoc = new rapidxml::xml_document<>();
}


TransactionHandler::TransactionHandler(): exchangeStorage(NULL) {    	
  mydoc = new rapidxml::xml_document<>();
}


TransactionHandler::~TransactionHandler(){}


ExchangeServerStorage& TransactionHandler::getExchangeStorage(){
  return this->exchangeStorage;
}

//creates XML for order creation
std::string TransactionHandler::create_order(std::string sym, int amount, double limit, int ID) {
  std::stringstream mystring;
  mystring<<"\t<opened sym=\""<<sym<<"\" id=\" "<<ID<<"\" limit="<<"\""<<limit<<"\"/>"<<"\n";
  return mystring.str();
}

//creates XML for execution
std::string TransactionHandler::execXML(int amount, int price, double time) {
  std::stringstream mystring;
  mystring<<"\t\t<executed shares=\""<<amount<<"\" price=\" "<<price<<"\" time="<<"\""<<time<<"\"/>"<<"\n";
  return mystring.str();
}

//creates XML for cancellation
std::string TransactionHandler::cancelXML(int amount, int price, double time) {
  std::stringstream mystring;
  mystring<<"\t\t<cancelled shares=\""<<amount<<"\" price=\" "<<price<<"\" time="<<"\""<<time<<"\"/>"<<"\n";
  return mystring.str();
}

//creates XML for opening
std::string TransactionHandler::openXML(int amount, int price, double time) {
  std::stringstream mystring;
  mystring<<"\t<opened shares=\""<<amount<<"\" price=\" "<<price<<"\" time="<<"\""<<time<<"\"/>"<<"\n";
  return mystring.str();
}


//status id 
std::string TransactionHandler::add_status(int ID) {
  std::stringstream mystring;
  mystring<<"\t<status id=\""<<ID<<"\"\n";
  return mystring.str();
}

//status end
std::string TransactionHandler::end_status(int ID) {
  std::stringstream mystring;
  mystring<<"</status>\n";
  return mystring.str();
}


//cancel id 
std::string TransactionHandler::add_cancel(int ID) {
std::stringstream mystring;
  mystring<<"\t<cancelled id=\""<<ID<<"\"\n";
  return mystring.str();
}

//cancel end
std::string TransactionHandler::end_cancel(int ID) {
  std::stringstream mystring;
  mystring<<"</canceleld>\n";
  return mystring.str();
}


//use for query or cancel
std::string TransactionHandler::error_id(int ID, int cond) {
  std::stringstream mystring;

 //1-->Order does not exist
  if (cond == 1) {
    mystring<<"\t<error id= "<<ID<<"/>"<<"Order does not exist: can't query"<<"</error>\n";
  }
  //2--> missing ID
  if (cond == 2) {
    mystring<<"\t<error"<<">"<<"Missing information to process order"<<"</error>\n";
  }
  return mystring.str();
}


//creates error XML depending on condition
std::string TransactionHandler::error_order(std::string sym, int amount, double limit, int cond) {
  std::stringstream mystring;

  //1--> missing any info: WORKS!
  if (cond == 1) {
    mystring<<"\t<error "<<">"<<"Missing information to process order"<<"</error>\n";
   }

  //2--> Account does not exist: WORKS!
  else if (cond == 2) {
    mystring<<"\t<error"<<">"<<"Account does not exist"<<"</error>\n";
  }
  //3--> Buy order won't go through: low balance
  else if (cond == 3) {
    mystring<<"\t<error "<<"sym=\""<<sym<<"\">balance too low"<<"</error>\n";
  }
 //4--> Buy order won't go through: low balance
  else if (cond == 4) {
    mystring<<"\t<error "<<"sym=\""<<sym<<"\">Not enough shares"<<"</error>\n";
  }
  //5--> 0 amount: not real buy/sale amount
  else if (cond == 5) {
    mystring<<"\t<error "<<"sym=\""<<sym<<"\">Need to specify more than 0 shares"<<"</error>\n";
  }
  //5--> 0 amount: not real buy/sale amount
  else if (cond == 6) {
    mystring<<"\t<error "<<"sym=\""<<sym<<"\">Account does not exist!"<<"</error>\n";
  }


  
  return mystring.str();
}


//gets int from const char * and checks for ERRORS 
int TransactionHandler::XML_to_int(const char* val) {
  std::stringstream strVal;
  int number;
  strVal << val;
  //Error checking
  if (strVal.str().empty()) {
    return ERRORNUM;
  }
  else {
    strVal >> number;
    return number;
  }
}

//gets int from const char * and checks for ERRORS 
double TransactionHandler::XML_to_double(const char* val) {
  std::stringstream strVal;
  double number;
  strVal << val;
  //Error checking
  if (strVal.str().empty()) {
    return ERRORNUM;
  }
  else {
    strVal >> number;
    return number;
  }
}



//reads XML --> makes Account instances 
void TransactionHandler::createNode(rapidxml::xml_node<>* myroot,
		std::shared_ptr<Transactions> contain,
		std::string& response,
		std::shared_ptr<Account> myacc,
		int counter) {
  //what we compare our attributes to determine if they are symbols or accounts
  char order[] = "order";
  char query[] = "query";
  char cancel[] = "cancel";
  int amount;
  double limit;
  std::string mystock;
  int ID;
  //Base Case for Recursion: exit function
  //std::cout<<"The counter value is:"<<counter<<std::endl;
  if (myroot == NULL) {
    return;
  }
  //Name is Order
  if (strcmp(myroot->name(), order) == 0) {

    //check if Account is NULL or not
    if (myacc.get()->get_ID() != -1) {
      
      //Save sym
      const char * stock = myroot->first_attribute()->value();
      //std::cout<<"The stock is:"<<stock<<std::endl;
      if (stock != NULL) {
	std::stringstream tunnel;
	tunnel<<stock;
	mystock = tunnel.str();
      }
      //Save amount
      const char * amt = myroot->first_attribute()->next_attribute()->value();
      amount = XML_to_int(amt);
      //std::cout<<"The value of the stock is:"<<amt<<std::endl;
      //Save limit
      const char * lim = myroot->first_attribute()->next_attribute()->next_attribute()->value();
      //std::cout<<"My limit is:"<<lim<<std::endl;
      limit = XML_to_double(lim);
      
      // No Parsing Errors
      if ((amount !=ERRORNUM)&&(limit != ERRORNUM)&&(!mystock.empty())) {
      
	//IF BUY order
	if (amount > 0) {
	  //check amount*limit < balance
	  if ((amount*limit) > myacc.get()->get_balance()) {
	    
	    //error
	    response.append(error_order(mystock, amount, limit, 3));
	  }
	  else {
	    std::shared_ptr<Order> myorder(new Order(myacc.get()->get_ID(),
						     mystock, amount, limit));
	    //line to add to database
	    bool buy = exchangeStorage.insertOrder(myorder);

	    if (buy) {
	      //append line to response
	      response.append(create_order(mystock, amount, limit, myacc.get()->get_ID()));
	    }
	    else {
	      response.append(error_order(mystock, amount, limit, 6));
	    }
	
	  }
	}
	else if (amount == 0) {
	  response.append(error_order(mystock, amount, limit, 5));
	}
	//Sell order
	else {
	  if (abs(amount) >myacc.get()->get_map()->find(mystock)->second) {
	    //error
	    response.append(error_order(mystock, amount, limit, 4));
	  }	  
	  else {
	    std::shared_ptr<Order> myorder(new Order(myacc.get()->get_ID(),
						     mystock, amount, limit));
	    //line to add to database
	    bool insert = exchangeStorage.insertOrder(myorder);
	    if (insert) {
	      response.append(create_order(mystock, amount, limit, myacc.get()->get_ID()));
	    }

	    //account ID does not exist
	    else {
	      response.append(error_order(mystock, amount, limit, 6));
	    }

	  }
	}   
      }
      //Parsing Error
      else {
	response.append(error_order(mystock, amount, limit, 1));
      }
    }
    //Null ACCOUNT: append errors
    else {
      response.append(error_order(mystock, amount, limit, 2));
    }   
  }
  //QUERY
  else if (strcmp(myroot->name(), query) == 0) {
    //check if id is filled
    const char * amt = myroot->first_attribute()->name();
    ID = XML_to_int(amt);
    //No error
    if (ID != ERRORNUM) {
      //add status id
      response.append(add_status(ID));
      //attemp to query it: return a list of Order objects
      std::list<std::shared_ptr<Order> > queries = exchangeStorage.queryOrdersByID(ID);
      //if list not empty -> append response depending on if executed, cancelled or opened 
      //use execXML(ID) or cancelXML(ID)
      if (queries.empty()) {
	response.append(error_id(ID, 1));
      }
      //if list is empty --> ERROR: condition 3
      else {
	for (std::list<std::shared_ptr<Order> >::iterator it = queries.begin(); it != queries.end(); ++it) {
	  //check if open
	  if ((*it).get()->get_open()) {
	    response.append(openXML((*it).get()->get_amount(), (*it).get()->get_limit(), (*it).get()->get_time()));
	  }
	  //check if executed
	  else if ((*it).get()->get_executed()) {
	    response.append(execXML((*it).get()->get_amount(), (*it).get()->get_limit(), (*it).get()->get_time()));
	  }
	  else {
	    //cancelled
	    response.append(cancelXML((*it).get()->get_amount(), (*it).get()->get_limit(), (*it).get()->get_time()));
	    
	  }
	}	
      //append end status
	response.append(end_status(ID));
      }
    }

    //ID not there
    else {
      response.append(error_id(ID, 2));
    }
  }
  //Cancel
    

  
  //Recurse to Child if they exist
  if (myroot->first_node() != NULL) {
    char *c = myroot->first_node()->name();
    if (c[0] != '\0') {
      counter++;
      createNode(myroot->first_node(), contain, response, myacc, counter);
    }
  }
  
  //Recurse to Siblings if they exist
  if (myroot->next_sibling() != NULL) {
    char *d = myroot->next_sibling()->name();
    if (d[0] != '\0') {
      counter++;
      createNode(myroot->next_sibling(), contain, response, myacc, counter);
    }
  }
}

std::string TransactionHandler::Parse_Preprocess(std::string buffer) {
  //response XML
  std::string response = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  response.append("<results>\n");
    
  //clear first line
  buffer.erase(0, buffer.find("\n")+1);
  std::shared_ptr<Transactions> mytransaction (new Transactions());


  //std::cout << "Test query by order id" << std::endl;
  this->exchangeStorage.queryOrdersByID(0);
  
  //make Account
  /*
  std::shared_ptr<Account> acc (new Account(12345, 10000));

  acc.get()->add_position("SPY", 100);
  acc.get()->add_position("TRY", 200);
  acc.get()->add_position("HIGH", 300);
  */
  
  int  counter = 0;
  char* cstr = new char[buffer.size() + 1];  // Create char buffer to store strings copy
  strcpy (cstr, buffer.c_str());             // Copy string into char buffer
  mydoc->parse<0>(cstr);
  //AFTER PARSE FIND OUT FIRST ATTRIBUTE
  char * account = mydoc->first_node()->first_attribute()->value();
  //std::cout<<"The account I found from this XML is:"<<account<<std::endl;
  //use the account you found to search in Accounts table
  int myacc = XML_to_int(account);
  std::shared_ptr<Account> acc = exchangeStorage.findAccount(myacc);

  //std::cout<<"My account info is:"<<std::endl;
  //std::cout<<*(acc.get());

  createNode(mydoc->first_node()->first_node(), mytransaction, response, acc, counter);
  response.append("<\\results>\n");

  return response;
}

