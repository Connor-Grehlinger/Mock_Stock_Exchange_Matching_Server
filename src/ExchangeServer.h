/***************************************************** 
 * Exchange Matching Server                          *
 * Developed by: Connor Grehlinger and Nisarg Dabhi  *
 *                                                   *
 *****************************************************/

// Standard C++/C libraries 
#include <iostream>

// Poco-specific libraries
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Timestamp.h>


// Implemented classes:
#include "ExchangeRequestHandlerFactory.h"
#include "ExchangeServerParams.h"



// Exchange Server class-- 
class ExchangeServer : public Poco::Net::TCPServer{
private:
 
  
public:
  
  ExchangeServer(ExchangeRequestHandlerFactory::Ptr eFactory,
		 Poco::ThreadPool& serverThreadPool,
		 const Poco::Net::ServerSocket& socket,
		 ExchangeServerParams::Ptr params);
  
  ~ExchangeServer();
  
  // TODO: functions for scalability testing 
  //For performance measurement:
  Poco::Timestamp exchangeServerStartTime;
  
};
//--


// ExchangeMatchingApplication class (app runs on ExchangeServer)--
class ExchangeMatchingApp : public Poco::Util::ServerApplication{

protected:
  int main(const std::vector<std::string> &);

};
//--


