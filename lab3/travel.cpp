#include "travel.h"
#include "database.h"

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Hash.h>

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


void TravelRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    if (request.getURI().find("/route/create") != -1) {

        DatabaseMongo db = DatabaseMongo::Get();
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

        Poco::JSON::Object::Ptr jsonTemplate = new Poco::JSON::Object();
        jsonTemplate->set("_id", id);
        Poco::MongoDB::Document doc;
        db.FillDocument(doc, jsonTemplate);

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

        Poco::JSON::Object::Ptr jsonTemplate = new Poco::JSON::Object();
        jsonTemplate->set("_id", id);
        Poco::MongoDB::Document doc;
        db.FillDocument(doc, jsonTemplate);

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

        Poco::JSON::Object::Ptr jsonTemplate = new Poco::JSON::Object();
        jsonTemplate->set("_id", id);
        
        Poco::MongoDB::Document doc;
        db.FillDocument(doc, jsonTemplate);

        cout << "doc to str " << doc.toString() << endl;

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
        // for(std::string& s : results) 
        //     result.push_back(fromJSON(s));

    }

    if (request.getURI().find("/travel/user_routes") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("user_id");
    }

    if (request.getURI().find("/travel/create") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string start = form.get("start");
        string route_id = form.get("route_id");
        string date = form.get("date");
    }

    if (request.getURI().find("/travel/modify") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());

        string id = form.get("id");
        string start = form.get("start");
        string route_id = form.get("route_id");
        string date = form.get("date");
    }

    if (request.getURI().find("/travel/delete") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
    }

    if (request.getURI().find("/travel/info") != -1) {
        DatabaseMongo &db = DatabaseMongo::Get();
        HTMLForm form(request, request.stream());
        
        string id = form.get("id");
    }
}