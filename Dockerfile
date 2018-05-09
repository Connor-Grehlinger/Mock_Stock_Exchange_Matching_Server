FROM gcc

RUN mkdir /src
WORKDIR /src

RUN mkdir /config
ADD ./config /config/

RUN apt-get update && apt-get install -y libpoco-dev libpqxx-dev valgrind libtinyxml2-dev 

ADD ./src /src

EXPOSE 5432

RUN chmod +x /config/entrypoint.sh
RUN chmod +rx ExchangeServer.cpp
