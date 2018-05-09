-------------- TESTING EXCHANGE SERVER SCALABILTY ------------------

	     - Connor Grehlinger, Nisarg Dabhi -

To run performance tests measuring scalability in the context of 
execution time under certain request volumes, make changes to the 
following macro fields:

    * NUM_REQ in /src/ExchangeRequestHandler.cpp
        - This sets the number of requests the Exchange 
        - Matching Server will handle before terminating
        - and printing the execution time it took to 
        - process those requests in microseconds and seconds
        - The total running time of the Exchange Server 
        - Application is also reported
        
    * NUM_THREADS in /testing/src/XMLRequestGen.cpp
        - This sets the number of threads which will be 
        - dispatched from a thread pool to perform a number
        - of iternations (defined by the NUM_REQUESTS macro)
        - of a function (called runTest) which load tests 
        - the Exchange Matching Server. In each iteration of 
        - the load testing function: 
            -- A new connection is established with the 
               Exchange Matching Server
            -- A request containing an XML string is sent to
               the Exchange Matching Server
            -- A reply sent by the Exchange Matching Server 
               is received by the calling thread
               
    * NUM_REQUESTS in /testing/src/XMLRequestGen.cpp
        - This specifies the number of iterations of the 
        - load testing function (runTest) each thread completes 
        - This means NUM_THREADS * NUM_REQUESTS is the total number
        - of connect + send + receive calls made to the Exchange 
        - Matching Server on each call to ./XMLRequestGen
        
    * The docker-compose.yml file in /testing/
        - Ensure that the bash command on line 6 iterates through
        - and calls the ./XMLRequestGen program enough times to 
        - geneate enough requests to hit the NUM_REQ macro threshold
        - in /src/ExchangeRequestHandler.cpp
    
(# XMLRequestGen calls * NUM_THREADS * NUM_REQUESTS) >= (NUM_REQ)  
        
-------------- TESTING EXCHANGE SERVER SCALABILTY ------------------
