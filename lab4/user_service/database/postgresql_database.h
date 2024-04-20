#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>

#include <Poco/Data/PostgreSQL/Connector.h>
#include <Poco/Data/PostgreSQL/PostgreSQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

using namespace std;

class Database{
    private:
        Database();
        string connection_string;
        unique_ptr<Poco::Data::SessionPool> pool;
        
    public:
        static Database& get();
        Poco::Data::Session create_session();
};

#endif