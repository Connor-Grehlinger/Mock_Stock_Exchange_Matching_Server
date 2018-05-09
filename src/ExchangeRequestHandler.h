#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include "XMLUtility.h"
#include <Poco/Timestamp.h>

class ExchangeRequestHandler : public Poco::Net::TCPServerConnection{
private:

  pqxx::connection * exchangeDBConnection;
  
  XMLUtility xmlUtility;
  
  static unsigned long next_count;
  
  unsigned long count;


  static Poco::Timestamp firstRequestTime;

 public:

  
  ExchangeRequestHandler(const Poco::Net::StreamSocket& socket, pqxx::connection * c);
  
  ~ExchangeRequestHandler();

  virtual void run();

  
};
