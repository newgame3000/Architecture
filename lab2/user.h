#ifndef USERHANDLER_H
#define USERHANDLER_H

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/JSON/Parser.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;

class UserRequestHandler : public HTTPRequestHandler {
    void handleRequest(HTTPServerRequest &request,  HTTPServerResponse &response);
};

class User{
    public:
        long id;
        string name;
        string surname;
        string login;
        string password;
        string phone;

        Poco::JSON::Object::Ptr toJSON();
};
#endif