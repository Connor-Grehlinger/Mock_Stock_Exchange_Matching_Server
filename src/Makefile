# You can compile with either gcc or g++
CC = g++
CFLAGS = -g -o 
OPTS = -O3 #-pg
LINKED = -lPocoNet -lPocoUtil -lPocoFoundation -lPocoCrypto -lPocoNetSSL -lssl -lcrypto -lpqxx -lpq -ltinyxml2
OBJ = ExchangeRequestHandler.o ExchangeRequestHandlerFactory.o ExchangeServer.o ExchangeServerParams.o XMLUtility.o CreateHandler.o ExchangeServerStorage.o account.o order.o create.o transactions.o TransactionHandler.o

all:
	$(CC) -c ExchangeServer.cpp -std=c++11 $(OPTS)
	$(CC) -c ExchangeRequestHandler.cpp -std=c++11 $(OPTS)
	$(CC) -c ExchangeRequestHandlerFactory.cpp -std=c++11 $(OPTS)
	$(CC) -c ExchangeServerParams.cpp -std=c++11 $(OPTS)
	$(CC) -c XMLUtility.cpp -std=c++11 $(OPTS)
	$(CC) -c CreateHandler.cpp -std=c++11 $(OPTS)
	$(CC) -c TransactionHandler.cpp -std=c++11 $(OPTS)
	$(CC) -c ExchangeServerStorage.cpp -std=c++11 $(OPTS)
	$(CC) -c account.cpp -std=c++11 $(OPTS)
	$(CC) -c order.cpp -std=c++11 $(OPTS)
	$(CC) -c create.cpp -std=c++11 $(OPTS)
	$(CC) -c transactions.cpp -std=c++11 $(OPTS)
	$(CC) $(CFLAGS) ExchangeServer $(OBJ) $(LINKED) -std=c++11 $(OPTS)

clean:
	rm ExchangeServer *~ 
