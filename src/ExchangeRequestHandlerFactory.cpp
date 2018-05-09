#include "ExchangeRequestHandlerFactory.h"
#include <iostream>


ExchangeRequestHandlerFactory::ExchangeRequestHandlerFactory(){
  //std::cout << "---- ExchangeRequestHandlerFactory created ----" << std::endl;
  try{ 
    this->exchangeDBConnection =
      new pqxx::connection("host=db user=postgres port=5432");

    if (this->exchangeDBConnection->is_open()){
      std::cout << "**** ExchangeHandlerFactory: Database " << this->exchangeDBConnection->dbname()
		<< " opened successfully ****" << std::endl;

      std::string dropAccount = "DROP TABLE IF EXISTS ACCOUNT;";
      std::string dropPosition = "DROP TABLE IF EXISTS POSITION;";
      std::string dropOpenOrder = "DROP TABLE IF EXISTS OPEN_ORDER;";
      std::string dropClosedOrder = "DROP TABLE IF EXISTS CLOSED_ORDER;";

      pqxx::work tableDrop(*(this->exchangeDBConnection));
      try{
	
	tableDrop.exec(dropPosition);
	tableDrop.exec(dropOpenOrder);
	tableDrop.exec(dropClosedOrder);
	tableDrop.exec(dropAccount); // dropping last b/c referenced by other tables
	tableDrop.commit();
      }
      catch(const pqxx::pqxx_exception& pqe){
	std::cerr << pqe.base().what() << std::endl;
	tableDrop.abort();
	std::cerr << "Could not drop old tables... shutting down" << std::endl;
	exit(EXIT_FAILURE);
      }
      
	
      std::string makeAccountTable = "CREATE TABLE ACCOUNT("
	"ACCOUNT_ID SERIAL PRIMARY KEY                     NOT NULL,"
	"BALANCE    INT                              NOT NULL);";

      
      std::string makePositionTable = "CREATE TABLE POSITION("
	"POSITION_ID SERIAL PRIMARY KEY                    NOT NULL, "
	"ACCOUNT_ID  INT REFERENCES ACCOUNT(ACCOUNT_ID) NOT NULL, "
	"POSITION_SYMBOL TEXT                           NOT NULL, "
	"VALUE           INT                         NOT NULL);";
      
      
      std::string makeOpenOrderTable = "CREATE TABLE OPEN_ORDER("
	"ORDER_ID   SERIAL PRIMARY KEY                     NOT NULL, "
	"ACCOUNT_ID INT REFERENCES ACCOUNT(ACCOUNT_ID)  NOT NULL, "
	"POSITION_SYMBOL TEXT                           NOT NULL, "
	"VALUE           INT                         NOT NULL, "
	"PRICE_LIMIT     INT                         NOT NULL, "
	"ORDER_TIME      TIMESTAMP                      NOT NULL);";

      std::string makeClosedOrderTable = "CREATE TABLE CLOSED_ORDER("
	"ORDER_ID   SERIAL PRIMARY KEY                     NOT NULL,"
	"ACCOUNT_ID INT REFERENCES ACCOUNT(ACCOUNT_ID)  NOT NULL,"
	"POSITION_SYMBOL TEXT                           NOT NULL,"
	"VALUE           INT                         NOT NULL,"
	"PRICE_LIMIT     INT                         NOT NULL,"
	"EXECUTED        BOOLEAN                        NOT NULL,"
	"CANCELED        BOOLEAN                        NOT NULL,"
	"ORDER_TIME      TIMESTAMP                      NOT NULL);";

      pqxx::work tableCreate(*(this->exchangeDBConnection));

      try{
	
	tableCreate.exec(makeAccountTable);
	tableCreate.exec(makePositionTable);
	tableCreate.exec(makeOpenOrderTable);
	tableCreate.exec(makeClosedOrderTable);	
	tableCreate.commit();
      }
      catch(const pqxx::pqxx_exception& pqe){
	std::cerr << pqe.base().what() << std::endl;
	tableCreate.abort();
	std::cerr << "Could not create tables... shutting down" << std::endl;
	exit(EXIT_FAILURE);
      }      
    }
    else{
      std::cerr << "Cannot open database connection for db "
		<< this->exchangeDBConnection->dbname()
		<< "... shutting down" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "pqxx exception: " << pqe.base().what() << std::endl; 
    std::cerr << "ExchangeServerStorage class could not connect to database"
	      << this->exchangeDBConnection->dbname()
	      << "... shuttind down" << std::endl;
    exit(EXIT_FAILURE);
  }

}

ExchangeRequestHandlerFactory::~ExchangeRequestHandlerFactory(){
  this->exchangeDBConnection->disconnect();
  std::cout << "ExchangeHandlerFactory destroyed, database disconnected"
	       << std::endl;
  delete this->exchangeDBConnection;
}


Poco::Net::TCPServerConnection* ExchangeRequestHandlerFactory::createConnection(const Poco::Net::StreamSocket& socket){

  return new ExchangeRequestHandler(socket, this->exchangeDBConnection);

}


  
