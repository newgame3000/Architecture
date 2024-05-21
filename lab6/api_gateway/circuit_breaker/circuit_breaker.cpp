#include "circuit_breaker.h"
#include <map>
#include <chrono>
#include <iostream>

using namespace std;

bool CircuitBreaker::check(const string &service_name) {

    if (services.find(service_name) == end(services)) {
        return true;
    }

    ServiceState &ss = services[service_name];

    cout << "circuit breaker: state [" << ((int)ss.state)<< "] fail [" << ss.fail_count << "] success [" << ss.success_count<< "]" << endl;

    switch (ss.state)
    {
    case State::close:
        return true;

    case State::semi_open:
        return true;

    case State::open:
        auto end = chrono::high_resolution_clock::now();
        double elapsed_seconds = chrono::duration<double>(end - ss.state_time).count();

        if (elapsed_seconds >= TIME_LIMIT)
        {
            cout << "circuit breaker: time limit reached" << endl;
            ss.state = State::semi_open;
            ss.success_count = 0;
            ss.fail_count = 0;
            return true;
        }

        return false;
    }
    return false;
}

void CircuitBreaker::fail(const string &service_name)
{
    if (services.find(service_name) == end(services))
    {
        ServiceState ss;
        ss.service = service_name;
        ss.state = State::close;
        ss.fail_count = 1;
        services[service_name] = ss;
    }
    else
    {
        ServiceState &ss = services[service_name];
        if (ss.state == State::close)
        {
            ss.state_time = chrono::high_resolution_clock::now();
            ++ss.fail_count;
            if (ss.fail_count > FAIL_COUNT){
                cout << "circuit breaker: error limit reached" << endl;
                ss.state = State::open;
            }
        } else
        if (ss.state == State::semi_open)
        {
            ss.state = State::open;
            ss.state_time = chrono::high_resolution_clock::now();
            ss.success_count = 0;
        }
    }
}

void CircuitBreaker::success(const string &service_name)
{
    if (services.find(service_name) != end(services))
    {
        ServiceState &ss = services[service_name];
        if (ss.state == State::semi_open)
        {
            ++ss.success_count;
            if (ss.success_count > SUCCESS_LIMIT)
            {
                cout << "circuit breaker: success limit reached" << endl;
                ss.state = State::close;
                ss.success_count = 0;
                ss.fail_count = 0;
            }
        } 
    }
}