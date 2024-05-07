#include "user.h"
#include "../database/postgresql_database.h"

#include <iostream>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Hash.h>
#include <Poco/Base64Decoder.h>

using namespace std;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

string generateToken(long &id, string &login) {
    Poco::JWT::Token token;
    token.setType("JWT");
    token.setSubject("login");
    token.payload().set("login", login);
    token.payload().set("id", id);
    token.setIssuedAt(Poco::Timestamp());

    Poco::JWT::Signer signer(getJWTKey());
    return signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
}

pair<string, string> getCredentials(const string identity) {
    istringstream istr(identity);
    ostringstream ostr;
    Poco::Base64Decoder b64in(istr);
    copy(istreambuf_iterator<char>(b64in),
         istreambuf_iterator<char>(),
         ostreambuf_iterator<char>(ostr));
    string decoded = ostr.str();

    size_t pos = decoded.find(':');
    string login = decoded.substr(0, pos);
    string password = decoded.substr(pos + 1);
    return {login, password};
}

string getJWTKey() {
    if (getenv("JWT_KEY") != nullptr) {
        cout << "key loaded" << endl;
        return getenv("JWT_KEY");
    }
    return "0123456789ABCDEF0123456789ABCDEF";
}

Poco::JSON::Object::Ptr User::toJSON()
{
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("id", id);
    root->set("name", name);
    root->set("surname", surname);
    root->set("login", login);
    root->set("password", password);
    root->set("phone", phone);

    return root;
}

void UserRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{

    if (request.getURI().find("/user/auth") != -1)
    {
        try
        {
            // HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement select(session);

            string scheme, info;
            request.getCredentials(scheme, info);
            cout << "scheme: " << scheme << " identity: " << info << endl;

            
            pair <string, string> logpass = getCredentials(info);
            string login = logpass.first;
            string password = logpass.second;

            Hash<string> hash;
            string passH = to_string(hash(password));

            // cout << "passH: " << passH;

            long id;

            select << "SELECT id FROM users WHERE login LIKE $1 AND pass LIKE $2",
                into(id),
                use(login),
                use(passH),
                range(0, 1);

            cout << "id = " << id << endl;
            select.execute();

            RecordSet rs(select);
            if (rs.moveFirst())
            {
                response.setStatus(HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                ostream &ostr = response.send();
                string token = generateToken(id, login);
                cout << "token: " << token << endl;
                ostr << "{ \"id\" : \"" << id << "\", \"Token\" : \""<< token <<"\"}" << endl;
            }
            else
            {
                cout << "Not found\n";
                response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("error", "Not found");
                ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
            }
            return;
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }

    if (request.getURI().find("/user/login") != -1)
    {
        try
        {
            HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement select(session);

            string login = form.get("login");

            cout << login << endl;

            User u;

            select << "SELECT id, name, surname, login, pass, phone FROM users WHERE login LIKE $1",
                into(u.id),
                into(u.name),
                into(u.surname),
                into(u.login),
                into(u.password),
                into(u.phone),
                use(login),
                range(0, 1);

            select.execute();

            RecordSet rs(select);
            if (rs.moveFirst())
            {

                response.setStatus(HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(u.toJSON(), ostr);
            }
            else
            {

                cout << "Not found\n";
                response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("error", "Not found");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
            }
            return;
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }

    if (request.getURI().find("/user/name") != -1)
    {

        try
        {
            HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement select(session);

            string name = form.get("name");
            string surname = form.get("surname");

            User u;

            select << "SELECT id, name, surname, login, pass, phone FROM users WHERE name LIKE $1 and surname LIKE $2",
                into(u.id),
                into(u.name),
                into(u.surname),
                into(u.login),
                into(u.password),
                into(u.phone),
                use(name),
                use(surname),
                range(0, 1);

            select.execute();

            RecordSet rs(select);
            if (rs.moveFirst())
            {

                response.setStatus(HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(u.toJSON(), ostr);
                return;
            }
            else
            {

                cout << "Not found\n";
                response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("error", "Not found");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }

    if (request.getURI().find("/user/register") != -1)
    {
        try
        {
            HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement insert(session);

            string name = form.get("name");
            string surname = form.get("surname");
            string login = form.get("login");
            Hash<string> hash;
            string pass = to_string(hash(form.get("pass")));
            string phone = form.get("phone");

            cout << "passH: " << pass << endl;


            User u;

            cout << "0\n";

            insert << "INSERT INTO users (name, surname, login, pass, phone) VALUES($1, $2, $3, $4, $5)",
                use(name),
                use(surname),
                use(login),
                use(pass),
                use(phone);

            insert.execute();

            cout << "1\n";

            int id = -1;

            Statement select(session);
            select << "SELECT LASTVAL()",
                into(id),
                range(0, 1);

            cout << "2\n";

            if (!select.done())
            {
                select.execute();
            }

            cout << "3\n";

            cout << "inserted:" << id << endl;

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            std::ostream &ostr = response.send();
            ostr << "{ \"id\" : \"" << id << "\"}" << std::endl;
            return;
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }

    if (request.getURI().find("/user/modify") != -1)
    {
        try
        {
            HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement insert(session);

            int id = atoi(form.get("id").c_str());
            string name = form.get("name");
            string surname = form.get("surname");
            string login = form.get("login");
            Hash<string> hash;
            unsigned long long passH = hash(form.get("pass"));
            string phone = form.get("phone");
            string pass = to_string(passH);

            User u;

            insert << "UPDATE users SET name = $1, surname = $2, login = $3, pass = $4, phone = $5 WHERE id = $6",
                use(name),
                use(surname),
                use(login),
                use(pass),
                use(phone),
                use(id);

            insert.execute();

            // cout << "OK";

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            std::ostream &ostr = response.send();
            ostr << "{ \"status\" : \""
                 << "OK"
                 << "\"}" << std::endl;
            return;
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }

    if (request.getURI().find("/user/delete") != -1)
    {
        try
        {
            HTMLForm form(request, request.stream());
            cout << "request.getURI(): " << request.getURI() << endl;

            Session session = Database::get().create_session();
            Statement insert(session);

            int id = atoi(form.get("id").c_str());

            User u;

            insert << "DELETE FROM users WHERE id = $1",
                use(id);

            insert.execute();

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            std::ostream &ostr = response.send();
            ostr << "{ \"status\" : \""
                 << "OK"
                 << "\"}" << std::endl;
            return;
        }

        catch (PostgreSQL::ConnectionException &e)
        {
            cout << "connection:" << e.what() << endl;
        }
        catch (PostgreSQL::StatementException &e)
        {
            cout << "statement:" << e.what() << endl;
        }
        return;
    }
}