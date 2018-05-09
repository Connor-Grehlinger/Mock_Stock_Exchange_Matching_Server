# You can compile with either gcc or g++
CC = g++
CFLAGS = -g -o
LINKED = -lPocoNet -lPocoUtil -lPocoFoundation -lPocoCrypto -lPocoNetSSL -lssl -lcrypto
OBJ = XMLRequestGen.o

all:
	$(CC) -c XMLRequestGen.cpp -std=c++11 -O3
	$(CC) $(CFLAGS) XMLRequestGen $(OBJ) $(LINKED) -std=c++11

clean:
	rm XMLRequestGen *~ 
