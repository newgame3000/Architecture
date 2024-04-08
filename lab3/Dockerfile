FROM ubuntu
ENV TZ=Europe/Moscow
RUN apt-get update && apt-get install -y git python3 pip iputils-ping cmake gcc-12 libpq-dev postgresql-client wrk libssl-dev zlib1g-dev librdkafka-dev mysql-client libmysqlclient-dev libboost-all-dev\
    && apt-get clean && pip install Faker 

RUN apt-get install -y wget    

RUN wget http://archive.ubuntu.com/ubuntu/pool/main/o/openssl/libssl1.1_1.1.0g-2ubuntu4_amd64.deb\
    && dpkg -i libssl1.1_1.1.0g-2ubuntu4_amd64.deb

RUN apt-get install -y gnupg curl

RUN curl -fsSL https://www.mongodb.org/static/pgp/server-4.4.asc |  gpg -o /usr/share/keyrings/mongodb-server-4.4.gpg    --dearmor
RUN echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-4.4.gpg ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/4.4 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-4.4.list
RUN apt-get update 

RUN apt-get -y install mongodb-org-tools
RUN apt-get -y install mongodb-org-shell

RUN git clone -b poco-1.13.3 https://github.com/pocoproject/poco.git &&\
    cd poco &&\
    mkdir cmake-build &&\
    cd cmake-build &&\
    cmake .. &&\
    cmake --build . --config Release &&\
    cmake --build . --target install &&\
    cd && rm poco/* -rf 

RUN ldconfig
ADD * /
RUN ./compile.sh
RUN python3 generate_data.py
ENTRYPOINT [ "./start.sh" ]