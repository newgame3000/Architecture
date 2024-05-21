#ifndef API_H
#define API_H

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/JSON/Parser.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;

class ApiRequestHandler : public HTTPRequestHandler {
    void handleRequest(HTTPServerRequest &request,  HTTPServerResponse &response);
};

std::pair<Poco::Net::HTTPResponse::HTTPStatus, string> getRequest(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth, const std::string &token, const std::string &body);
std::string getCached(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth);
std::string getKey(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth);
void putCache(const std::string &method, const std::string &base_path, const std::string &query, const std::string &basic_auth,const std::string &result);

#endif