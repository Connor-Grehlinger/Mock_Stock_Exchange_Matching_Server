#include <Poco/Net/TCPServerParams.h>
#include <Poco/SharedPtr.h>


// Parameter class which configures the Exchange Server --
class ExchangeServerParams : public Poco::Net::TCPServerParams{

public:
  //typedef Poco::SharedPtr<ExchangeServerParams> Ptr;

  ExchangeServerParams();

  ~ExchangeServerParams();
  

};
// --


