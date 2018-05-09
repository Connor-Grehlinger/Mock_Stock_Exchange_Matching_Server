#include "ExchangeServer.h"

#define MAX_QUEUE 128
#define MAX_THREADS 4096
#define MIN_THREADS 8

ExchangeServer::ExchangeServer(ExchangeRequestHandlerFactory::Ptr eFactory,
			       Poco::ThreadPool& serverThreadPool,
			       const Poco::Net::ServerSocket& socket,
			       ExchangeServerParams::Ptr params):
  Poco::Net::TCPServer(eFactory, serverThreadPool, socket, params),
  exchangeServerStartTime(Poco::Timestamp()){
  // Timestamp made to measure run time for request handling    
  
  
  //std::cout << "**** ExchangeServer constructed ****" << std::endl; 
}


ExchangeServer::~ExchangeServer(){
  //std::cout << "**** ExchangeServer destroyed ****" << std::endl;  
}



int ExchangeMatchingApp::main(const std::vector<std::string> &){

  // args: min capacity, max capacity, idle timeout, initial stack size
  Poco::ThreadPool exchangeThreadPool(MIN_THREADS, MAX_THREADS, 60, 0);

  // The Exchange Server will handle all incoming connections and assigns a new
  // or existing thread to each connection (threads are managed by the thread pool)

  ExchangeServerParams * exchangeServerParams = new ExchangeServerParams();
  exchangeServerParams->setMaxQueued(MAX_QUEUE);
  
  Poco::Net::ServerSocket exchangeSocket(12345);
  exchangeSocket.setKeepAlive(true);

  ExchangeServer exchangeServer(new ExchangeRequestHandlerFactory(),
				exchangeThreadPool,
				exchangeSocket,
				exchangeServerParams);

  exchangeServer.start();

  //std::cout << "**** Exchange matching server started ****" << std::endl;
  //std::cout << "Current thread count: " << exchangeServer.currentThreads() << std::endl;
  //std::cout << "Max thread count set at: " << exchangeServer.maxThreads() << std::endl;

  Poco::Util::ServerApplication::waitForTerminationRequest();
  
  Poco::Timestamp::TimeDiff runtime = exchangeServer.exchangeServerStartTime.elapsed();

  exchangeServer.stop();

  exchangeThreadPool.joinAll();
  
  std::cout << "Exchange matching server shutting down..." << std::endl;
  std::cout << "@@@ Total up time (us) = " << runtime << " @@@" << std::endl;
  std::cout << "@@@ Total up time (seconds) = "
	    << ((double)runtime/(double)1000000) << " @@@" << std::endl;
  
  return Poco::Util::Application::EXIT_OK;
}



/* Main function to run the Exchange Matching Application + Server */
int main(int argc, char *argv[]){
  ExchangeMatchingApp app;
  return app.run(argc, argv);
}

