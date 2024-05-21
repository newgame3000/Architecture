cmake_minimum_required(VERSION 3.2)

project(TravelServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g")
set(SOURCES main.cpp handler/travel.cpp handler/travel.h database/mongo_database.h database/mongo_database.cpp)

find_package(Threads)
find_package(OpenSSL)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL Data JWT MongoDB)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}.out ${SOURCES})

target_link_libraries(${PROJECT_NAME}.out PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES} "PocoDataPostgreSQL")
