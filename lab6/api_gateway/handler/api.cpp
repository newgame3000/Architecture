#include "api.h"
#include "../database/cache.h"
#include "../circuit_breaker/circuit_breaker.h"

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>

using namespace std;

string getKey(const string &method, const string &base_path, const string &query, const string &basic_auth){
    return method + ":" + base_path + ":" + query + ":" + basic_auth;
}

string getCached(const string &method, const string &base_path, const string &query, const string &basic_auth) {

    string key =  getKey(method, base_path, query, basic_auth);

    std::cout << "key: " << key << endl;

    string result;

    auto c = Cache::get();
    cout << "first get cache\n";
    if(Cache::get().get(key, result)){
        std::cout << "result: " << result << endl;
        return result;
    } else {
        return string();
    }
}

void putCache(const string &method, const string &base_path, const string &query, const string &basic_auth, const string &result) {
    string key =  getKey(method, base_path, query, basic_auth);
    Cache::get().put(key,result);
}

pair<Poco::Net::HTTPResponse::HTTPStatus, string> getRequest(const string &method, const string &base_path, const string &query, const string &basic_auth, const string &token, const string &body) {
    
    try
    {        
        // URL для отправки запроса
        Poco::URI uri(base_path + query);

        string path(uri.getPathAndQuery());
        if (path.empty()) {
            path = "/";
        }

        cout << "api gateway: request " << base_path + query << endl;

        // Создание сессии HTTP
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

        // Создание запроса
        Poco::Net::HTTPRequest request(method, path, Poco::Net::HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");;

        if (!basic_auth.empty())
        {
            request.set("Authorization", "Basic " + basic_auth);
        }
        else if (!token.empty())
        {
            request.set("Authorization", "Bearer " + token);
        }

        cout << "sending: " << request.getURI() << endl;

        // Отправка запроса
        session.sendRequest(request);

        // Получение ответа
        Poco::Net::HTTPResponse response;
        istream &rs = session.receiveResponse(response);

        // Вывод ответа на экран
        stringstream ss;
        Poco::StreamCopier::copyStream(rs, ss);

        return pair<Poco::Net::HTTPResponse::HTTPStatus, string>({response.getStatus(), ss.str()});
    }
    catch (Poco::Exception &ex)
    {
        cerr << ex.displayText() << endl;
        return pair<Poco::Net::HTTPResponse::HTTPStatus, string>({Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND, ""});
    }
}

void ApiRequestHandler::handleRequest(HTTPServerRequest &request,  HTTPServerResponse &response) {

    static CircuitBreaker circuit_breaker;
    string base_user_url = getenv("USER_ADRESS");
    string base_travel_url = getenv("TRAVEL_ADRESS");

    if (request.getURI().find("/user/") != -1) {
        cout << "without login pass\n";
        Poco::URI uri(request.getURI());
        
        pair<Poco::Net::HTTPResponse::HTTPStatus, string> res = getRequest(Poco::Net::HTTPRequest::HTTP_GET, base_user_url, uri.getPathAndQuery(), "", "", "");
        
        response.setStatus(res.first);
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");

        cout << "res: " << res.second << endl;
        response.send() << res.second;
        return;
    }

    string scheme;
    string info;
    request.getCredentials(scheme, info);

    cout << "api gateway - scheme: " << scheme << " identity: " << info << endl; 

    if (scheme == "Basic") {

        pair<Poco::Net::HTTPResponse::HTTPStatus, string> ans = getRequest(Poco::Net::HTTPRequest::HTTP_GET, base_user_url, "/user/auth", info, "", "");
        cout << "api gateway - auth :" << ans.second << endl;

        if (!ans.second.empty()) {

            if(circuit_breaker.check("travel_server")) {
                Poco::JSON::Parser parser;
                Poco::Dynamic::Var result = parser.parse(ans.second);
                Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

                string token = object->getValue<string>("Token");

                cout << "token: " << token << endl;

                pair<Poco::Net::HTTPResponse::HTTPStatus, string> real_result = getRequest(request.getMethod(), base_travel_url, request.getURI(), "", token, "");
                cout << "api gateway - result: " << endl << real_result.second << endl;
                if(!real_result.second.empty()){
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    ostream &ostr = response.send();
                    ostr << real_result.second;
                    ostr.flush();
                    putCache(request.getMethod(), base_travel_url, request.getURI(), info, real_result.second);
                    circuit_breaker.success("travel_server");
                } else {
                    cout << "fail" << real_result.second << endl;
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_UNAUTHORIZED);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                    root->set("type", "/errors/unauthorized");
                    root->set("title", "Internal exception");
                    root->set("status", "401");
                    root->set("detail", "not authorized");
                    root->set("instance", "/user/auth");
                    ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(root, ostr);
                    circuit_breaker.fail("travel_server");
                }
                
            } else {

                if(request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
                    string cache_result = getCached(request.getMethod(), base_travel_url, request.getURI(),info);
                    if(!cache_result.empty()){
                        cout << "api gateway - from cache : " << cache_result << endl;
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        ostream &ostr = response.send();
                        ostr << cache_result;
                        ostr.flush();
                        return;
                    } else {
                        cout << "cache_empty\n";
                    }
                }

                std::cout << "api gateway - fail to request from cache" << std::endl;
                response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_SERVICE_UNAVAILABLE);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/unavailable");
                root->set("title", "Service unavailable");
                root->set("status", "503");
                root->set("detail", "circuit breaker open");
                root->set("instance", request.getURI());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);     

            }
        }
    }
}