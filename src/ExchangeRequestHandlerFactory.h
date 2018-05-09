#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/StreamSocket.h>
#include <pqxx/pqxx>
#include <pqxx/except.hxx>
#include "ExchangeRequestHandler.h"

// Creates handlers for exchange matching --
class ExchangeRequestHandlerFactory : public Poco::Net::TCPServerConnectionFactory{
private:
  pqxx::connection * exchangeDBConnection;

  
public:

  ExchangeRequestHandlerFactory();

  ~ExchangeRequestHandlerFactory();

  
  virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);

};
// --



