#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>

#include <Poco/MongoDB/MongoDB.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Database.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/JSON/Object.h>

using namespace std;

class DatabaseMongo{
    private:    
        DatabaseMongo();
        Poco::MongoDB::Connection connection_mongo;
        Poco::MongoDB::Database database_mongo;
                
    public:
        static DatabaseMongo &Get();
        Poco::MongoDB::Database &GetMongoDatabase();
        void SendToMongo(const string& collection, Poco::JSON::Object::Ptr json);
        void UpdateMongo(const string& collection, Poco::MongoDB::Document &docr, Poco::JSON::Object::Ptr json);
        void DeleteInMongo(const string& collection, Poco::MongoDB::Document &docr);
        void FillDocument(Poco::MongoDB::Document &doc, Poco::JSON::Object::Ptr &obj);
        std::vector<string> GetFromMongo(const string& collection, Poco::MongoDB::Document &docr);
        // long count_from_mongo(const string& collection,map<string,long>& params);
};

#endif