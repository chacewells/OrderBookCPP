#include <iostream>
#include <sqlite3.h>

#define ORDER_BOOK_DB_FILE "orderbook.sqlite3"
#define SCHEMA_SQL "CREATE TABLE orderbook (" \
"id INTEGER PRIMARY KEY, " \
"ticker_symbol TEXT, " \
"side TEXT, " \
"amount INTEGER, " \
"price NUMERIC, " \
"total NUMERIC);"


int main() {
    // get sqlite3 handle
    sqlite3 *db;
    sqlite3_open(ORDER_BOOK_DB_FILE, &db);

    // create schema
    char *errmsg;
    sqlite3_exec(db, SCHEMA_SQL, nullptr, nullptr, &errmsg);
    if (errmsg) {
        std::cerr << errmsg << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // release sqlite3 resource
    sqlite3_close(db);

    return 0;
}
