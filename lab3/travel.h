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

// class Route{
//     public:
//     string id;
//     string start;
//     string end;

//     Route fromJSON(string str);
//     Poco::JSON::Object::Ptr toJSON();
// };
#endif