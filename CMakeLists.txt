cmake_minimum_required(VERSION 3.14)
project(HashTable_PassServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(HashTable_PassServer main.cpp hashtable.h hashtable.hpp passserver.h passserver.cpp)