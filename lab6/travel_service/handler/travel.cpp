#include "travel.h"
#include "../database/mongo_database.h"

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Hash.h>
#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"
#include "Poco/Base64Decoder.h"

#include <sstream>
#include <ctime>

using namespace std;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

Poco::JSON::Object::Ptr routeToJSON(string str)
{
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("_id", object->getValue<string>("_id"));
    root->set("start", object->getValue<string>("start"));
    root->set("end", object->getValue<string>("end"));

    return root;
}

Poco::JSON::Object::Ptr travelToJSON(string str)
{
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("_id", object->getValue<string>("_id"));
    root->set("users", object->getArray("users"));
    root->set("route_id", object->getValue<string>("route_id"));
    root->set("date", object->getValue<string>("date"));

    return root;
}

bool ExtractPayloadFromToken(string &jwt_token, int &id, string &login) {
    if (jwt_token.length() == 0) {
        return false;
    }
    Poco::JWT::Signer signer(getJWTKey());
    try {
        Poco::JWT::Token token = signer.verify(jwt_token);
        if (token.payload().has("login") && token.payload().has("id")) {
            login = token.payload().getValue<string>("login");
            id = token.payload().getValue<int>("id");
            return true;
        }
        cout << "Not enough fields in token" << endl;

    } catch (...) {
        cout << "Token verification failed" << endl;
    }
    return false;
}

string getJWTKey() {
    if (getenv("JWT_KEY") != nullptr) {
        cout << "key loaded" << endl;
        return getenv("JWT_KEY");
    }
    return "0123456789ABCDEF0123456789ABCDEF";
}

void TravelRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    if (request.getURI().find("/route/create") != -1) {

        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string start = form.get("start");
        string end = form.get("end");

        //Так как лабораторная делается на одной машине и одном потоке, 
        //можно использовать в качестве идентификатора только время, остальное же
        //заполнить нулями

        ostringstream ss;
        ss << setfill('0') << setw(4) << hex << time(nullptr);

        string id = ss.str() + "000000000000000";

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();

        jsonRequest->set("_id", id);
        jsonRequest->set("start", start);
        jsonRequest->set("end", end);

        db.SendToMongo("routes", jsonRequest);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"id\" : \"" << id << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/route/modify") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string id = form.get("id");
        string start = form.get("start");
        string end = form.get("end");

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();
        jsonRequest->set("_id", id);
        jsonRequest->set("start", start);
        jsonRequest->set("end", end);

        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        db.UpdateMongo("routes", doc, jsonRequest);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/route/delete") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        db.DeleteInMongo("routes", doc);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/route/id") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
        
        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        std::vector<string> results = db.GetFromMongo("routes", doc);

        if (results.size() == 0) {
            cout << "Not found\n";
            response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
            root->set("error", "Not found");
            ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);
            return;
        }

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();

        for (int i = 0; i < results.size(); ++i) {
            Poco::JSON::Stringifier::stringify(routeToJSON(results[i]), ostr);
        }

        return;
    }

    if (request.getURI().find("/travel/create") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string route_id = form.get("route_id");
        string date = form.get("date");

        string scheme, token;
        request.getCredentials(scheme, token);
        cout << "scheme: " << scheme << " identity: " << token << endl;
        // string token = form.get("user").c_str();

        int user_id = 0;
        string login;
        ExtractPayloadFromToken(token, user_id, login);

        cout << "user_id: " << user_id << endl;

        ostringstream ss;
        ss << setfill('0') << setw(4) << hex << time(nullptr);

        string id = ss.str() + "000000000000000";

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();
        Poco::Dynamic::Array arr;
        arr.push_back(user_id);

        jsonRequest->set("_id", id);
        jsonRequest->set("users", arr);
        jsonRequest->set("route_id", route_id);
        jsonRequest->set("date", date);

        db.SendToMongo("travels", jsonRequest);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"id\" : \"" << id << "\"}" << endl;

        return;
    }

    if (request.getURI().find("/travel/modify") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string id = form.get("id");
        
        string scheme, token;
        request.getCredentials(scheme, token);
        cout << "scheme: " << scheme << " identity: " << token << endl;
        
        string route_id = form.get("route_id");
        string date = form.get("date");

        int user_id = 0;
        string login;
        ExtractPayloadFromToken(token, user_id, login);

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();
        Poco::Dynamic::Array arr;
        arr.push_back(user_id);

        jsonRequest->set("_id", id);
        jsonRequest->set("users", arr);
        jsonRequest->set("route_id", route_id);
        jsonRequest->set("date", date);

        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        db.UpdateMongo("travels", doc, jsonRequest);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/travel/delete") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        db.DeleteInMongo("travels", doc);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/travel/id") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");

        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        std::vector<string> results = db.GetFromMongo("travels", doc);

        if (results.size() == 0) {
            cout << "Not found\n";
            response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
            root->set("error", "Not found");
            ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);
            return;
        }

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();

        for (int i = 0; i < results.size(); ++i) {
            Poco::JSON::Stringifier::stringify(travelToJSON(results[i]), ostr);
        }

        return;
    }

    if (request.getURI().find("/travel/add_user") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
        string scheme, token;
        request.getCredentials(scheme, token);
        cout << "scheme: " << scheme << " identity: " << token << endl;

        int user_id = 0;
        string login;
        ExtractPayloadFromToken(token, user_id, login);

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();
        jsonRequest->set("users", user_id);
        Poco::JSON::Object::Ptr jsonRequest2 = new Poco::JSON::Object();
        jsonRequest2->set("$addToSet", jsonRequest);

        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        db.UpdateMongo("travels", doc, jsonRequest2);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/travel/user_delete") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string id = form.get("id");
        Poco::MongoDB::Document doc;
        doc.add("_id", id);

        string scheme, token;
        request.getCredentials(scheme, token);
        cout << "scheme: " << scheme << " identity: " << token << endl;

        int user_id = 0;
        string login;
        ExtractPayloadFromToken(token, user_id, login);

        Poco::JSON::Object::Ptr jsonRequest = new Poco::JSON::Object();
        jsonRequest->set("users", user_id);
        Poco::JSON::Object::Ptr jsonRequest2 = new Poco::JSON::Object();
        jsonRequest2->set("$pull", jsonRequest);

        db.UpdateMongo("travels", doc, jsonRequest2);

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();
        ostr << "{ \"status\" : \"" << "OK" << "\"}" << endl;
        return;
    }

    if (request.getURI().find("/travel/user_routes") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string scheme, token;
        request.getCredentials(scheme, token);
        cout << "scheme: " << scheme << " identity: " << token << endl;

        int user_id = 0;
        string login;
        ExtractPayloadFromToken(token, user_id, login);

        Poco::MongoDB::Document doc;
        doc.add("users", user_id);

        std::vector<string> results = db.GetFromMongo("travels", doc);

        if (results.size() == 0) {
            cout << "Not found\n";
            response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
            root->set("error", "Not found");
            ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);
            return;
        }

        response.setStatus(HTTPResponse::HTTP_OK);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");
        ostream &ostr = response.send();

        Poco::JSON::Object::Ptr ans = new Poco::JSON::Object();

        for (int i = 0; i < results.size(); ++i) {
            ans->set("route_" + to_string(i + 1), travelToJSON(results[i])->get("route_id"));
        }
        Poco::JSON::Stringifier::stringify(ans, ostr);

        return;
    }
}