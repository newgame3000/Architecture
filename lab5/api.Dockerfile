FROM ubuntu
ENV TZ=Europe/Moscow
RUN apt-get update && apt-get install -y git python3 pip iputils-ping cmake gcc-12 libpq-dev postgresql-client wrk libssl-dev zlib1g-dev librdkafka-dev mysql-client libmysqlclient-dev libboost-all-dev\
    && apt-get clean

RUN git clone -b poco-1.13.3 https://github.com/pocoproject/poco.git &&\
    cd poco &&\
    mkdir cmake-build &&\
    cd cmake-build &&\
    cmake .. &&\
    cmake --build . --config Release &&\
    cmake --build . --target install &&\
    cd && rm poco/* -rf

RUN git clone https://github.com/tdv/redis-cpp.git &&\
    cd redis-cpp &&\
    mkdir build &&\ 
    cd build &&\ 
    cmake .. &&\ 
    make &&\ 
    make install &&\
    cd ..     

RUN ldconfig
ADD /api_gateway /
RUN ./compile.sh
ENTRYPOINT [ "./ApiGateway.out" ]