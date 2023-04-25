//
// Created by chace on 4/25/2023.
//

#include <iostream>
#include "OrderBookManager.h"

#define ORDER_BOOK_DB_FILE "orderbook.sqlite3"

#define ORDER_BOOK_DDL "CREATE TABLE IF NOT EXISTS orderbook (" \
"id INTEGER PRIMARY KEY, " \
"ticker_symbol TEXT, " \
"side TEXT, " \
"amount INTEGER, " \
"price NUMERIC, " \
"total NUMERIC);"

#define CREATE_ORDER_SQL "INSERT INTO orderbook (ticker_symbol, side, amount, price, total) " \
"values (?, ?, ?, ?, ?);"

namespace orderbook {
    OrderBookManager::OrderBookManager() {
        sqlite3_open(ORDER_BOOK_DB_FILE, &db);
        std::cout << "successfully connected to " << ORDER_BOOK_DB_FILE << std::endl;
    }

    int OrderBookManager::bootstrap_schema() {
        std::cout << "running DDL [" << ORDER_BOOK_DDL << "]\n";
        int res = sqlite3_exec(db, ORDER_BOOK_DDL, nullptr, nullptr, nullptr);
        std::cout << "DDL " << (res ? "fail" : "success") << std::endl;
        return res;
    }

    void OrderBookManager::create(orderbook::Order &new_order) {
        sqlite3_stmt *stmt;
        int res = sqlite3_prepare_v2(db, CREATE_ORDER_SQL, std::string(CREATE_ORDER_SQL).size(), &stmt, nullptr);
        if (res) {
            std::cerr << "failed to prepare statement: " << sqlite3_errstr(res) << std::endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, new_order.ticker_symbol.c_str(), new_order.ticker_symbol.size(), nullptr);
        sqlite3_bind_text(stmt, 2, new_order.side.c_str(), new_order.side.size(), nullptr);
        sqlite3_bind_int(stmt, 3, new_order.amount);
        sqlite3_bind_double(stmt, 4, new_order.price);
        sqlite3_bind_double(stmt, 5, new_order.total);

        res = sqlite3_step(stmt);
        if (res != SQLITE_DONE) {
            std::cerr << "error executing statement: " << sqlite3_errstr(res) << std::endl;
            std::cerr << "error code " << res << std::endl;
        }

        new_order.id = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
    }

    OrderBookManager::~OrderBookManager() {
        sqlite3_close(db);
        std::cout << "successfully closed db connection\n";
    }

    std::ostream &operator<<(std::ostream &o, const Order &order) {
        o << "Order{ " << order.id
          << ", " << order.ticker_symbol
          << ", " << order.side
          << ", " << order.amount
          << ", " << order.price
          << ", " << order.total
          << " }";
        return o;
    }
} // orderbook