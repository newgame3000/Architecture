#include "cache.h"

#include <exception>
#include <mutex>

#include <redis-cpp/stream.h>
#include <redis-cpp/execute.h>

using namespace std;

mutex _mtx;
Cache::Cache()
{
    string host = getenv("CACHE_HOST");
    string port = getenv("CACHE_PORT");

    cout << "cache host:" << host <<" port:" << port << endl;
    _stream = rediscpp::make_stream(host, port);
}

Cache Cache::get()
{
    static Cache instance;
    return instance;
}

void Cache::put([[maybe_unused]] const string &id, [[maybe_unused]] const string &val)
{
    lock_guard<mutex> lck(_mtx);
    rediscpp::value response = rediscpp::execute(*_stream, "set",  id, val, "ex", "60");
    cout << "put_cache\n";
}

bool Cache::get([[maybe_unused]] const string & id, [[maybe_unused]] string &val)
{
    try {

        lock_guard<mutex> lck(_mtx);
        rediscpp::value response = rediscpp::execute(*_stream, "get", id);

        if (response.is_error_message()) {
            cout << "error\n";
            return false;
        }

        if (response.empty()) {
            cout << "empty\n";
            return false;
        }

        val = response.as<string>();
        return true;

    } catch(...) {
        return false;
    }
}