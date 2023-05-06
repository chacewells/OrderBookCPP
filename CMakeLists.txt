cmake_minimum_required(VERSION 3.25)
project(OrderBookCPP)

set(CMAKE_CXX_STANDARD 17)

add_executable(OrderBookCPP main.cpp OrderBookManager.cpp OrderBookManager.h OrderSimulator.cpp OrderSimulator.h)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(OrderBookCPP PRIVATE unofficial::sqlite3::sqlite3)

file(COPY sp500.txt DESTINATION ${CMAKE_BINARY_DIR})