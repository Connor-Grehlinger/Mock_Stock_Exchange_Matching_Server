#include "CreateHandler.h"
#include "TransactionHandler.h"

#include <vector>
#include <pqxx/pqxx>
#include <pqxx/except.hxx>
#include <Poco/Mutex.h>



class XMLUtility{
private:
  std::vector<std::string> xmlRequests;

  std::string xmlRequest;

  //Poco::Mutex lock;
  
  pqxx::connection * exchangeDBConnection;

  CreateHandler createHandler;

  TransactionHandler transactionHandler;

public:
  XMLUtility();

  XMLUtility(pqxx::connection * c);

  ~XMLUtility();

  CreateHandler& get_create(void);

  TransactionHandler& get_trans(void);

  int create_or_transaction(std::string mystring);
  
  std::string addXmlRequest(char * xmlReq);
  

  std::string getXmlResponse(void); // may need argument, we'll see

};


