#ifndef USERHANDLER_H
#define USERHANDLER_H

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/JSON/Parser.h>
#include "Poco/JWT/Token.h"
#include "Poco/JWT/Signer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

class UserRequestHandler : public HTTPRequestHandler {
    void handleRequest(HTTPServerRequest &request,  HTTPServerResponse &response);
};

string getJWTKey();
string generateToken(long &id, string &login);
pair<string, string> getCredentials(const string identity);

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