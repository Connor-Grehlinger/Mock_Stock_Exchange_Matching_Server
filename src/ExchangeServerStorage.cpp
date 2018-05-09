#include "ExchangeServerStorage.h"
#include <cstdlib>
#include <string>
#include <map>
ExchangeServerStorage::ExchangeServerStorage(pqxx::connection* c):
  exchangeDBConnection(c){
}

ExchangeServerStorage::~ExchangeServerStorage(){
}


void ExchangeServerStorage::printAccounts(){
  pqxx::work getAccounts(*(this->exchangeDBConnection));
  try{
    std::string getAccountsQuery = "SELECT * FROM ACCOUNT;";
  
    pqxx::result r(getAccounts.exec(getAccountsQuery));

    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      std::cout << "ACCOUNT_ID: " << it[0].as<int>() << ", BALANCE: "
		<< it[1].as<long>() << std::endl;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying ACCOUNT table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
  }
}


void ExchangeServerStorage::printPositions(){
  pqxx::work getPositions(*(this->exchangeDBConnection));

  try{
    std::string getPositionsQuery = "SELECT * FROM POSITION;";
  
    pqxx::result r(getPositions.exec(getPositionsQuery));

    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      std::cout << "POSITION_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()
		<< ", VALUE: " << it[3].as<int>() << std::endl;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying POSITION table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
  }
}

 
//Order has: 1. accID 2.symbol 3.amount 4. limit --> order ID, CLOSED bool and time are generated
//on their own
bool ExchangeServerStorage::insertPosition(std::shared_ptr<Account>& account, std::string sym,  int value){

  pqxx::work insertAccount(*(exchangeDBConnection));
  
  std::string accToInsert = "INSERT INTO POSITION (ACCOUNT_ID,POSITION_SYMBOL,VALUE)"
    " VALUES (" + std::to_string(account->get_ID()) + ", " +
    "\'"+sym+"\'" + ", " + std::to_string(value) + ");";
  
  try{
    insertAccount.exec(accToInsert);
    insertAccount.commit();
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << pqe.base().what() << std::endl;
    return false;
  }
  return true;
}



bool ExchangeServerStorage::insertAccount(std::shared_ptr<Account>& account){

  pqxx::work insertAccount(*(this->exchangeDBConnection));
  
  std::string accToInsert = "INSERT INTO ACCOUNT (ACCOUNT_ID,BALANCE)"
    " VALUES (" + std::to_string(account->get_ID()) + ", " +
    std::to_string(account->get_balance()) + ");";
  
  try{
    insertAccount.exec(accToInsert);
    insertAccount.commit();
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << pqe.base().what() << std::endl;
    return false;
  }
  return true;
}


std::shared_ptr<Account> ExchangeServerStorage::findOrder(int ID) {
  std::shared_ptr<Account> empty (new Account(-1, -1));
  try{
    int account;
    int balance;
    std::string find = "SELECT * FROM ORDER WHERE ACCOUNT_ID = "
      +std::to_string(ID)+";";

    pqxx::work findAccount(*(this->exchangeDBConnection));
    pqxx::result r(findAccount.exec(find));

    if (r.empty() ) {
      std::cout<<"I AM EMPTY!"<<std::endl;
      return empty;
    }
    
    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      account = it[0].as<int>();
      balance = it[1].as<int>();
    }
   
    return std::shared_ptr<Account> (new Account(account, balance));
    
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying ACCOUNT table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
    //std::shared_ptr<Account> empty (new Account(-1, -1));
    return empty;
  }
}




std::shared_ptr<Account> ExchangeServerStorage::findAccount(int ID) {
  std::shared_ptr<Account> empty (new Account(-1, -1));
  try{
    int account;
    int balance;
    std::string find = "SELECT * FROM ACCOUNT WHERE ACCOUNT_ID = "
      +std::to_string(ID)+";";

    pqxx::work findAccount(*(this->exchangeDBConnection));
    pqxx::result r(findAccount.exec(find));

    if (r.empty() ) {
      std::cout<<"I AM EMPTY!"<<std::endl;
      return empty;
    }
    
    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      account = it[0].as<int>();
      balance = it[1].as<int>();
    }
   
    return std::shared_ptr<Account> (new Account(account, balance));
    
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying ACCOUNT table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
    //std::shared_ptr<Account> empty (new Account(-1, -1));
    return empty;
  }
}



std::map<int,std::shared_ptr<Order> > ExchangeServerStorage::Buy_List() {
  pqxx::work getOrders(*(this->exchangeDBConnection));
  std::map<int, std::shared_ptr<Order> > buylist;
  try{

    int order_ID;
    int acc_ID;	
    std::string sym;
    int amount;
    double limit;
    double time;
    
    std::string getBuysQuery = "SELECT * FROM OPEN_ORDER WHERE VALUE > 0;";
  
    pqxx::result r(getOrders.exec(getBuysQuery));

    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      order_ID = it[0].as<int>();
      acc_ID = it[1].as<int>();
      sym = it[2].as<std::string>();
      amount = it[3].as<int>();
      limit = it[4].as<double>();
      time = it[5].as<double>();
      std::shared_ptr<Order> myorder(new Order(acc_ID, order_ID, sym, amount, limit, time));

 /*
      std::cout << "ORDER_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()	       << ", VALUE: " << it[3].as<int>() << ", PRICE_LIMIT: "<< it[4].as<int>() << ", ORDER_TIME: "<< it[5].as<int>() << std::endl;
      */
      //add to buylist
      buylist.emplace(order_ID, myorder);
      return buylist;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying POSITION table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
    //return empty list
    return buylist;
  }
}

std::map<int,std::shared_ptr<Order> > ExchangeServerStorage::Sell_List() {
  pqxx::work getOrders(*(this->exchangeDBConnection));
  std::map<int, std::shared_ptr<Order> > buylist;
  try{

    int order_ID;
    int acc_ID;	
    std::string sym;
    int amount;
    double limit;
    double time;
    
    std::string getBuysQuery = "SELECT * FROM OPEN_ORDER WHERE VALUE < 0;";
  
    pqxx::result r(getOrders.exec(getBuysQuery));

    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      order_ID = it[0].as<int>();
      acc_ID = it[1].as<int>();
      sym = it[2].as<std::string>();
      amount = it[3].as<int>();
      limit = it[4].as<double>();
      time = it[5].as<double>();
      std::shared_ptr<Order> myorder(new Order(acc_ID, order_ID, sym, amount, limit, time));

 /*
      std::cout << "ORDER_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()	       << ", VALUE: " << it[3].as<int>() << ", PRICE_LIMIT: "<< it[4].as<int>() << ", ORDER_TIME: "<< it[5].as<int>() << std::endl;
      */

      //add to buylist
      buylist.emplace(order_ID, myorder);
      return buylist;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying POSITION table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
    //return empty list
    return buylist;
  }
}


bool ExchangeServerStorage::insertOrder(std::shared_ptr<Order>& order){

  pqxx::work insertOrd(*(exchangeDBConnection));
  
  std::string orderToInsert = "INSERT INTO OPEN_ORDER (ACCOUNT_ID,POSITION_SYMBOL,VALUE, PRICE_LIMIT,ORDER_TIME)"
    " VALUES (" + std::to_string(order->get_accID()) + ", " +
    "\'"+order->get_sym()+"\'" + ", " + std::to_string(order->get_amount()) + ", " + std::to_string(order->get_limit()) + ", " + "CURRENT_TIMESTAMP" + ");";
  
  try{
    insertOrd.exec(orderToInsert);
    insertOrd.commit();
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << pqe.base().what() << std::endl;
    return false;
  }
  return true;
}



void ExchangeServerStorage::printOpenOrder(){
  pqxx::work getPositions(*(this->exchangeDBConnection));

  try{
    std::string getPositionsQuery = "SELECT * FROM OPEN_ORDER;";
  
    pqxx::result r(getPositions.exec(getPositionsQuery));

    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      std::cout << "ORDER_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()<< ", VALUE: " << it[3].as<int>() << ", PRICE_LIMIT: "<< it[4].as<int>() << ", ORDER_TIME: "<< it[5].as<int>() << std::endl;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying POSITION table" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
  }
}


std::list<std::shared_ptr<Order> > ExchangeServerStorage::queryOrdersByID(int ID){
  pqxx::work getOrders(*(this->exchangeDBConnection));
  std::list<std::shared_ptr<Order> > resultList;
  
  int order_ID;
  int acc_ID;	
  std::string sym;
  int amount;
  double limit;
  double time;
  bool executed;
  bool canceled;

  
  try{
    /*
    std::string getOrdersQuery = "SELECT OPEN_ORDER.ORDER_ID, OPEN_ORDER.ACCOUNT_ID, "
      "OPEN_ORDER.POSITION_SYMBOL, OPEN_ORDER.VALUE, OPEN_ORDER.PRICE_LIMIT, OPEN_ORDER.ORDER_TIME, "
      "CLOSED_ORDER.ORDER_ID, CLOSED_ORDER.ACCOUNT_ID, CLOSED_ORDER.POSITION_SYMBOL, "
      "CLOSED_ORDER.VALUE, CLOSED_ORDER.PRICE_LIMIT, CLOSED_ORDER.EXECUTED, CLOSED_ORDER.CANCELED, "
      "CLOSED_ORDER.ORDER_TIME FROM OPEN_ORDER, CLOSED_ORDER WHERE (OPEN_ORDER.ORDER_ID = "
      ""+ std::to_string(ID) + ") OR (CLOSED_ORDER.ORDER_ID = " + std::to_string(ID) + ");";
    */
    // just doing closed order functionality, not constructing from all fields 

    std::string getOrdersQuery = "SELECT * FROM CLOSED_ORDER WHERE CLOSED_ORDER.ORDER_ID"
      " = " + std::to_string(ID) + ";";
    
    pqxx::result r(getOrders.exec(getOrdersQuery));
    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
      order_ID = it[0].as<int>();
      acc_ID = it[1].as<int>();
      sym = it[2].as<std::string>();
      amount = it[3].as<int>();
      limit = it[4].as<double>();
      executed = it[5].as<bool>();
      canceled = it[6].as<bool>();
      time = it[7].as<double>();
      //std::shared_ptr<Order> myorder(new Order(acc_ID, order_ID, sym, amount, limit, time));
      resultList.push_front(std::shared_ptr<Order>(new Order(acc_ID,
							     order_ID,
							     sym,
							     amount,
							     limit,
							     time,
							     canceled,
							     executed)));

      std::cout << "ORDER_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()
		<< ", VALUE: " << it[3].as<int>() << ", PRICE_LIMIT: "
		<< it[4].as<int>() << ", EXECUTED: " << it[5].as<bool>() << ", CANCELED: "
		<< it[6].as<bool>() << ", ORDER_TIME: "<< it[7].as<int>() << std::endl; 						
    }							  

  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying orders by ID" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
  }
  return resultList;
}

std::shared_ptr<Order>  ExchangeServerStorage::findClosedOrder(int ID){
  pqxx::work getOrders(*(this->exchangeDBConnection));
  std::shared_ptr<Order> empty (new Order(-1, "IDK", -1, -1));  
  int order_ID;
  int acc_ID;	
  std::string sym;
  int amount;
  double limit;
  double time;
  bool executed;
  bool canceled;
  
  
  try{
    /*
    std::string getOrdersQuery = "SELECT OPEN_ORDER.ORDER_ID, OPEN_ORDER.ACCOUNT_ID, "
      "OPEN_ORDER.POSITION_SYMBOL, OPEN_ORDER.VALUE, OPEN_ORDER.PRICE_LIMIT, OPEN_ORDER.ORDER_TIME, "
      "CLOSED_ORDER.ORDER_ID, CLOSED_ORDER.ACCOUNT_ID, CLOSED_ORDER.POSITION_SYMBOL, "
      "CLOSED_ORDER.VALUE, CLOSED_ORDER.PRICE_LIMIT, CLOSED_ORDER.EXECUTED, CLOSED_ORDER.CANCELED, "
      "CLOSED_ORDER.ORDER_TIME FROM OPEN_ORDER, CLOSED_ORDER WHERE (OPEN_ORDER.ORDER_ID = "
      ""+ std::to_string(ID) + ") OR (CLOSED_ORDER.ORDER_ID = " + std::to_string(ID) + ");";
    */
    // just doing closed order functionality, not constructing from all fields 

    std::string getOrdersQuery = "SELECT * FROM CLOSED_ORDER WHERE CLOSED_ORDER.ORDER_ID"
      " = " + std::to_string(ID) + ";";
    
    pqxx::result r(getOrders.exec(getOrdersQuery));
    if (r.empty()) {
      return empty;
    }
    for (pqxx::result::const_iterator it = r.begin(); it != r.end(); ++it){
    order_ID = it[0].as<int>();
    acc_ID = it[1].as<int>();
    sym = it[2].as<std::string>();
    amount = it[3].as<int>();
    limit = it[4].as<double>();
    executed = it[5].as<bool>();
    canceled = it[6].as<bool>();
    time = it[7].as<double>();
    std::shared_ptr<Order> myorder(new Order(acc_ID, order_ID, sym, amount, limit, time));
      std::cout << "ORDER_ID: " << it[0].as<int>() << ", ACCOUNT_ID: "
		<< it[1].as<int>() << ", POSITION_SYMBOL: " << it[2].as<std::string>()
		<< ", VALUE: " << it[3].as<int>() << ", PRICE_LIMIT: "
		<< it[4].as<int>() << ", EXECUTED: " << it[5].as<bool>() << ", CANCELED: "
		<< it[6].as<bool>() << ", ORDER_TIME: "<< it[7].as<int>() << std::endl;
      return myorder;
    }
  }
  catch (const pqxx::pqxx_exception& pqe){
    std::cerr << "Error querying orders by ID" << std::endl;
    std::cerr << pqe.base().what() << std::endl;
    return empty;
  }
}
