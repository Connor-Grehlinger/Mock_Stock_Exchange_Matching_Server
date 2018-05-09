// Poco-specific libraries
#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>
#include <Poco/ThreadPool.h>
#include <Poco/Timespan.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketAddress.h>
#include <string>
#include <iostream>
#include "XMLRequestGen.h"

#define NUM_THREADS 1
#define NUM_REQUESTS 1
#define BUFF_SIZE 4096

// Each thread to carry out this method
void runTest(void){

  int requestOption = 0;
  
  Poco::UInt16 port = 12345;

  Poco::Timespan requestTimeOut(10,10000000);
  
  for (int i = 0; i < NUM_REQUESTS; i++){
  
    Poco::Net::SocketAddress Exchange("vcm-3817.vm.duke.edu", port);
  
    Poco::Net::StreamSocket exchangeSocket(Exchange);

    exchangeSocket.setSendTimeout(requestTimeOut);
    exchangeSocket.setReceiveTimeout(requestTimeOut);
    
    exchangeSocket.setKeepAlive(true);

    std::string testDataStream;
    
    switch (requestOption){

    case 0:
 
      testDataStream =
	"173\r\n"
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
	"<create>\r\n"
	"  <account id=\"12345\" balance=\"1000\"/>\r\n"
	"  <account id=\"1\" balance=\"200\"/>\r\n"
	"  <account id=\"2\" balance=\"300\"/>\r\n"
	"  <symbol sym=\"SPY\">\r\n"
	"    <account id=\"12345\">100000</account>\r\n"
	"  </symbol>\r\n"
	"  <symbol sym=\"SPY\">\r\n"
	"    <account id=\"1\">979</account>\r\n"
	"    <account id=\"2\">356</account>\r\n"
	"  </symbol>\r\n"
	"</create>\r\n\r\n";
      break;
    

    case 1:

      testDataStream =
	"173\r\n"
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
	"<transactions account = \"12345\">\r\n"
	"<order sym =\"SPY\" amount=\"100\" limit=\"145.67\"/>\r\n"
	"<order sym =\"XYZ\" amount=\"99\" limit=\"42.00\"/>\r\n"
	"<order sym =\"ABC\" amount=\"-42\" limit=\"1.43\"/>\r\n"
	"</transactions>\r\n\r\n";
      break;
      
    default:
      testDataStream =
	"173\r\n"   
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
	"<create>\r\n"
	"  <account id=\"12000\" balance=\"1000\"/>\r\n"
	"  <symbol sym=\"WOMBO\">\r\n"
	"    <account id=\"12000\">100000</account>\r\n"
	"  </symbol>\r\n"
	"</create>\r\n\r\n";
 
    }

      
    const char * sendReq = testDataStream.c_str();
  
    size_t bytesSent = exchangeSocket.sendBytes(sendReq, strlen(sendReq) + 1);

    std::cout << "Send request of " << bytesSent << " to exchange server" << std::endl;

    char replyBuffer[BUFF_SIZE];
    memset(replyBuffer, 0, BUFF_SIZE);
  
    size_t bytesRecv = exchangeSocket.receiveBytes(replyBuffer, BUFF_SIZE);
    
    std::cout << "Client received " << bytesRecv << " bytes from the exchange "
	      << "server with message: " << std::endl;
    
    std::cout << replyBuffer << std::endl;
      
  }
  
}



int main(int argc, char *argv[]){

  Poco::ThreadPool testThreadPool(NUM_THREADS, 2048, 60, 0);

  Poco::ThreadTarget threadReq(runTest);
  
  for (int i = 0; i < NUM_THREADS; i++){
    testThreadPool.start(threadReq);
  }
  
  //std::cout << "Number of threads allocated = " << testThreadPool.allocated()
  //<< std::endl;
  //std::cout << "Number of threads capacity = " << testThreadPool.capacity()
  //<< std::endl;
  //std::cout << "Number of threads used = " << testThreadPool.used() << std::endl;
  
  testThreadPool.joinAll();
  //std::cout << "Number of threads allocated = " << testThreadPool.allocated()
  //<< std::endl;
  //std::cout << "Number of threads capacity = " << testThreadPool.capacity()
  //<< std::endl;
  //std::cout << "Number of threads used = " << testThreadPool.used()
  //<< std::endl;
  
  return 0;  
}


