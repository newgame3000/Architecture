#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>

#include <iostream>
#include "handler/travel.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;

class Factory : public HTTPRequestHandlerFactory {
public:
    Factory(const string &format): format(format) {} 

    HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request) {
        if (request.getURI().find("/route") != -1 || request.getURI().find("/travel") != -1 ) {
            return new TravelRequestHandler;
        }    
        return 0;
    }
private:
    string format;
};

class HTTPWebServer : public ServerApplication {
protected:
    int main([[maybe_unused]] const std::vector<std::string> &args)
    {
            ServerSocket ss(8081); // does bind + listen
            HTTPServer srv(new Factory(DateTimeFormat::SORTABLE_FORMAT), ss, new HTTPServerParams);
            srv.start();
            waitForTerminationRequest();
            srv.stop();

        return Application::EXIT_OK;
    }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    HTTPWebServer app;
    return app.run(argc, argv);
}