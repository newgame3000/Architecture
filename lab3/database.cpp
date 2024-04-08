#include "database.h"

#include <Poco/MongoDB/Array.h>


DatabaseMongo::DatabaseMongo(): database_mongo("archdb") {

    string mongo = getenv("MONGO_DB_HOST");
    string mongo_port = getenv("MONGO_DB_PORT");;

    connection_mongo.connect(mongo, atoi(mongo_port.c_str()));
}

Poco::MongoDB::Database &DatabaseMongo::GetMongoDatabase() {
    return database_mongo;
}

DatabaseMongo &DatabaseMongo::Get()
{
    static DatabaseMongo instance;
    return instance;
}

void DatabaseMongo::UpdateMongo(const string& collection, Poco::MongoDB::Document &docr, Poco::JSON::Object::Ptr json) {
    try {
        cout << "update" << endl;
        Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request = database_mongo.createUpdateRequest(collection);
        request->selector() = docr;
        Poco::MongoDB::Document &doc = request->update();
        cout << "aaa.toString()" << endl;
        FillDocument(doc, json);
        cout << doc.toString() << endl;
        connection_mongo.sendRequest(*request); 
    } 
    catch(exception &ex) {
        cout << "mongodb exception: " << ex.what() << endl;
        string lastError = database_mongo.getLastError(connection_mongo);
        if (!lastError.empty()) {
            cout << "mongodb Last Error: " << lastError << endl;
        }
    }
    cout << "OK\n";
}

std::vector<string> DatabaseMongo::GetFromMongo(const string& collection, Poco::MongoDB::Document &docr) {
    vector<string> result;
    try { 
        Poco::SharedPtr<Poco::MongoDB::QueryRequest> request = database_mongo.createQueryRequest(collection);
        Poco::MongoDB::ResponseMessage response;

        request->selector() = docr;

        connection_mongo.sendRequest(*request, response);

        for (auto& doc: response.documents()) {
            result.push_back(doc->toString());
        }
    }
    catch(exception &ex) {
        cout << "mongodb exception: " << ex.what() << endl;
        string lastError = database_mongo.getLastError(connection_mongo);
        if (!lastError.empty()) {
            cout << "mongodb Last Error: " << lastError << endl;
        }
    }

    return result;
}

void DatabaseMongo::SendToMongo(const string &collection, Poco::JSON::Object::Ptr json) {
    try {
        Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertRequest = database_mongo.createInsertRequest(collection);
        Poco::MongoDB::Document &doc = insertRequest->addNewDocument();
        Poco::MongoDB::ResponseMessage response;  
        FillDocument(doc, json);
        connection_mongo.sendRequest(*insertRequest);
        cout << "OK\n";
    }
    catch (exception &ex) {
        cout << "mongodb exception: " << ex.what() << endl;
        string lastError = database_mongo.getLastError(connection_mongo);
        if (!lastError.empty()) {
            cout << "mongodb Last Error: " << lastError << endl;
        }
    }
}

void DatabaseMongo::DeleteInMongo(const string& collection, Poco::MongoDB::Document &docr) {
    try {
        Poco::SharedPtr<Poco::MongoDB::DeleteRequest> deleteRequest = database_mongo.createDeleteRequest(collection);
        deleteRequest->selector() = docr;
        connection_mongo.sendRequest(*deleteRequest);
        cout << "OK\n";
    }
    catch (exception &ex) {
        cout << "mongodb exception: " << ex.what() << endl;
        string lastError = database_mongo.getLastError(connection_mongo);
        if (!lastError.empty()) {
            cout << "mongodb Last Error: " << lastError << endl;
        }
    }
}

void DatabaseMongo::FillDocument(Poco::MongoDB::Document &doc, Poco::JSON::Object::Ptr &obj) {
    for (auto &val : *obj) {

        if (val.second.isInteger()) {
            int v;
            val.second.convert(v);
            doc.add(val.first, v);
        }

        else if (val.second.isString()) {
            doc.add(val.first, val.second.extract<string>());
        } else if (val.second.isArray()) {
            Poco::MongoDB::Array &ar = doc.addNewArray(val.first);
            for (auto &valar : val.second) {
                ar.add(valar.extract<int>());
            }
        } else {
            try
            {
                Poco::JSON::Object::Ptr child = val.second.extract<Poco::JSON::Object::Ptr>();
                Poco::MongoDB::Document &child_doc = doc.addNewDocument(val.first);
                FillDocument(child_doc, child);
            }
            catch (...)
            {
                doc.add(val.first, val.second.toString());
            }
        }
    }
};

