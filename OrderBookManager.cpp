//
// Created by chace on 4/25/2023.
//

#include <iostream>
#include <string>
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

#define CANCEL_ORDER_SQL "DELETE FROM orderbook WHERE id = ?"

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
            sqlite3_finalize(stmt);
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
        } else {
            new_order.id = sqlite3_last_insert_rowid(db);
        }

        sqlite3_finalize(stmt);
    }

    void OrderBookManager::cancel_by_id(const int &id) {
        sqlite3_stmt *stmt;
        int res = sqlite3_prepare_v2(db, CANCEL_ORDER_SQL, std::string(CANCEL_ORDER_SQL).size(), &stmt, nullptr);
        if (res) {
            std::cerr << "failed to prepare statement: " << sqlite3_errstr(res) << std::endl;
            sqlite3_finalize(stmt);
            return;
        }

        sqlite3_bind_int(stmt, 1, id);

        res = sqlite3_step(stmt);
        if (res != SQLITE_DONE) {
            std::cerr << "error executing statement: " << sqlite3_errstr(res) << std::endl;
            std::cerr << "error code " << res << std::endl;
        }

        sqlite3_finalize(stmt);
    }

    OrderBookManager::~OrderBookManager() {
        sqlite3_close(db);
        std::cout << "successfully closed db connection\n";
    }

    std::vector<Order> OrderBookManager::get_orders_by_symbol(const std::string &ticker_symbol) {
        const std::string sql = "SELECT id, side, amount, price, total FROM orderbook WHERE ticker_symbol = ? "
                                "ORDER BY total, side ASC";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, ticker_symbol.c_str(), ticker_symbol.size(), nullptr);

        std::vector<Order> results;

        int res;
        res = sqlite3_step(stmt);
        while (res != SQLITE_DONE) {
            int id = sqlite3_column_int(stmt, 0);
            std::string side {(char*)sqlite3_column_text(stmt, 1)};
            int amount = sqlite3_column_int(stmt, 2);
            double price = sqlite3_column_double(stmt, 3);
            double total = sqlite3_column_double(stmt, 4);

            results.push_back({id, ticker_symbol, side, amount, price, total});

            res = sqlite3_step(stmt);
        }

        sqlite3_finalize(stmt);
        return std::move(results);
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