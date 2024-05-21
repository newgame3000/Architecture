#ifndef TRAVEL_H
#define TRAVEL_H

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/JSON/Parser.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;

class TravelRequestHandler : public HTTPRequestHandler {
    void handleRequest(HTTPServerRequest &request,  HTTPServerResponse &response);
};

Poco::JSON::Object::Ptr routeToJSON(string str);
Poco::JSON::Object::Ptr travelToJSON(string str);
bool ExtractPayloadFromToken(string &jwt_token, int &id, string &login);

string getJWTKey();

#endif