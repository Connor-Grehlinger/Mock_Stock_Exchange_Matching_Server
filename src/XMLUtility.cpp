#include "XMLUtility.h"
#include <cstdlib>
#include <string>
#include <iostream>

XMLUtility::XMLUtility():exchangeDBConnection(NULL),
			 createHandler(NULL),
			 transactionHandler(NULL){
}

XMLUtility::XMLUtility(pqxx::connection * c):exchangeDBConnection(c),
					     createHandler(c),
					     transactionHandler(c){

}


XMLUtility::~XMLUtility(){
}



int XMLUtility::create_or_transaction(std::string mystring) {
  rapidxml::xml_document<> doc;
  std::string edit = mystring;
  edit.erase(0, edit.find("\n")+1);
  char * cstr = new char[edit.size() +1];
  strcpy(cstr, edit.c_str());
  doc.parse<0>(cstr);
  if (strcmp("create", doc.first_node()->name()) == 0) {
    //std::cout<<"This is a create!"<<std::endl;
    return 0;
  }
  else if (strcmp("transactions", doc.first_node()->name()) == 0) {
    //std::cout<<"This is a transaction!"<<std::endl;
    return 1;
  }
  else {
    //std::cout<<"Unknown tag!"<<std::endl;
    return 2;
  }
}


CreateHandler& XMLUtility::get_create(void) {
  return createHandler;
}

TransactionHandler& XMLUtility::get_trans(void) {
  return transactionHandler;
}


std::string XMLUtility::getXmlResponse(void){
  return this->xmlRequests[0];
}




std::string XMLUtility::addXmlRequest(char * xmlReq){
  
  //this->lock.lock();
  
  std::string toParse(xmlReq);
  
  //std::cout << "--- String received by addXmlRequest: ---" << std::endl;
  //std::cout << toParse << std::endl;

  std::string response("Nothing done");
  
  if (create_or_transaction(toParse) == 0) {
    response = this->createHandler.Parse_Preprocess(toParse);
    //std::cout<<"Going to print Accounts!"<<std::endl;
    //createHandler.getExchangeStorage().printAccounts();
    //std::cout<<"Going to print Positionss!"<<std::endl;
    //createHandler.getExchangeStorage().printPositions();
  }
  else if (create_or_transaction(toParse) == 1) {
    response = transactionHandler.Parse_Preprocess(toParse);
    // matching logic: exchange share, modify accounts, etc 
  }
  else {
    //std::cout<<"We have an unknown tag!"<<std::endl;
  }
  
  // test db coherence
  //this->lock.unlock();


  return response;
}
