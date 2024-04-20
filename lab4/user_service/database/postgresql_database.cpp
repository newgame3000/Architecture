#include "postgresql_database.h"

Database::Database(){
    string host = getenv("DB_HOST");
    string user = getenv("DB_LOGIN");
    string dbname = getenv("DB_DATABASE");
    string port = getenv("DB_PORT");
    string password = getenv("DB_PASSWORD");

    connection_string = "host=" + host + " user=" + user + " dbname="
    + dbname + " port=" + port + " password=" + password;

    cout << "Connection string:" << connection_string << endl;
    Poco::Data::PostgreSQL::Connector::registerConnector();
    pool = make_unique<Poco::Data::SessionPool>(Poco::Data::PostgreSQL::Connector::KEY, connection_string);
}

Database& Database::get(){
    static Database instance;
    return instance;
}

Poco::Data::Session Database::create_session(){
    return Poco::Data::Session(pool->get());
}