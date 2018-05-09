# ECE590 Engineering Robust Server Software 
-- Homework 4: Exchange Matching Server --
Developed by Connor Grehlinger (cmg88) and Nasarg Dabhi (nrd10)


Written in C++, exchanged messages are in XML format


## Deloyment
To run the exchange matching server, enter the following commands:
```bash
$ sudo docker-compose build
$ sudo docker-compose up
```
If you run into the issue of the docker container timing out from
an error similar to:

ERROR: An HTTP request took too long to complete. Retry with --verbose to 
obtain debug information. If you encounter this issue regularly because of 
slow network conditions, consider setting COMPOSE_HTTP_TIMEOUT to a higher
value (current value: 60)

Try running: 
```bash
$ sudo COMPOSE_HTTP_TIMEOUT=300 docker-compose up 
``` 
Or whatever value allows the Exchange Matching Server to remain running 
during testing.


