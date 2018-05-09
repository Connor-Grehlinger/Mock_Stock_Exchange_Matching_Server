#include "ExchangeRequestHandler.h"
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/NetException.h>
#include <Poco/Exception.h>
#include <Poco/StreamCopier.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Process.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#define BUFF_SIZE 4096
//#define NUM_REQ 16000 // for terminating server for scalability testing 

ExchangeRequestHandler::ExchangeRequestHandler(const Poco::Net::StreamSocket& socket,
					       pqxx::connection * c):
  Poco::Net::TCPServerConnection(socket),
  exchangeDBConnection(c),
  xmlUtility(c){
  this->count = this->next_count;
  this->next_count++;
  //std::cout << "-- Request Handler " << this->count << " created --" << std::endl;
}

ExchangeRequestHandler::~ExchangeRequestHandler(){
  //std::cout << "-- Request Handler " << this->count << " destoryed --" << std::endl;
}


void ExchangeRequestHandler::run(){
  // For measuring execution time 
  if (this->count == 1){
    this->firstRequestTime = Poco::Timestamp();
  }

  std::string response("Request received by exchange server, not processed\n");
  
  try{
    
    char xmlReqBuffer[BUFF_SIZE];
    memset(xmlReqBuffer, 0, BUFF_SIZE);

    this->socket().setKeepAlive(true);
    
    size_t readBytes = this->socket().receiveBytes(xmlReqBuffer, BUFF_SIZE);

    
    //std::cout << readBytes << " bytes read into buffer of size "<< BUFF_SIZE
    //<< std::endl;
    
    //std::cout << "strlen(xmlReqBuffer) = " << strlen(xmlReqBuffer) << std::endl;
    // strlen == readBytes-1 for future reference 

    char * xmlReq = new char[readBytes + 1]; // + 1 for NULL char
    memset(xmlReq, 0, readBytes + 1);
    memcpy(xmlReq, xmlReqBuffer, readBytes + 1);
    
    response = this->xmlUtility.addXmlRequest(xmlReq);

    delete[] xmlReq;
    
  }
  catch (const Poco::Net::NetException& pne){
    std::cerr << pne.what() << std::endl;
    
  }

  const char * to_client = response.c_str();
  
  size_t sentBytes = this->socket().sendBytes(to_client, strlen(to_client) + 1);

  //std::cout << sentBytes << " bytes sent to client" << std::endl;

  //std::cout << this->count << std::endl;

  
  //NOTE: uncomment below if you would like to terminate the server application
  //      after a certain number of requests (used for scalability testing)
  
  /*
  if (this->count == NUM_REQ){
    Poco::Timestamp::TimeDiff trueRunTime = this->firstRequestTime.elapsed();
    std::cout << "Exchange Match Server processing time (us) = " << trueRunTime
	      << std::endl;
    std::cout << "Exchange Match Server processing time (seconds) = "
	      << ((double)trueRunTime/(double)1000000) << std::endl;
    std::cout << NUM_REQ << " requests were processed" << std::endl;
    
    Poco::Process::requestTermination(Poco::Process::id());    
  }
  */
  
}

unsigned long ExchangeRequestHandler::next_count = 1;

Poco::Timestamp ExchangeRequestHandler::firstRequestTime = Poco::Timestamp();
