cmake_minimum_required(VERSION 3.2)

project(ApiGateway)

set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g  ${REDISCPP_FLAGS}")
set(SOURCES main.cpp handler/api.cpp handler/api.h database/cache.h database/cache.cpp circuit_breaker/circuit_breaker.h circuit_breaker/circuit_breaker.cpp)
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")

find_package(Threads)
find_package(OpenSSL)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL Data JWT)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}.out ${SOURCES})

target_link_libraries(${PROJECT_NAME}.out PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})
