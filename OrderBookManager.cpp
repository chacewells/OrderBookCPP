//
// Created by chace on 4/25/2023.
//

#include <iostream>
#include <string>
#include <iomanip>
#include "OrderBookManager.h"

#define ORDER_BOOK_DB_FILE "orderbook.sqlite3"

#define ORDER_BOOK_DDL "CREATE TABLE IF NOT EXISTS orderbook (" \
"id INTEGER PRIMARY KEY, " \
"ticker_symbol TEXT, " \
"side TEXT, " \
"amount INTEGER, " \
"price NUMERIC, " \
"total NUMERIC);"

#define CREATE_ORDER_SQL "INSERT INTO orderbook (ticker_symbol, side, amount, price, total) "
#define ORDER_VALUES_SQL "values (?, ?, ?, ?, ?)"
#define ORDER_VALUES_CONT_SQL ", (?, ?, ?, ?, ?)"

#define CANCEL_ORDER_SQL "DELETE FROM orderbook WHERE id = ?"

#define GET_ORDERS_BY_SYMBOL_SQL "SELECT id, side, amount, price, total FROM orderbook WHERE ticker_symbol = ? " \
"ORDER BY total, side ASC"

using namespace std;

OrderBookManager::OrderBookManager() {
    sqlite3_open(ORDER_BOOK_DB_FILE, &db);
    cout << "successfully connected to " << ORDER_BOOK_DB_FILE << endl;
}

int OrderBookManager::bootstrap_schema() {
    cout << "running DDL [" << ORDER_BOOK_DDL << "]\n";
    int res = sqlite3_exec(db, ORDER_BOOK_DDL, nullptr, nullptr, nullptr);
    cout << "DDL " << (res ? "fail" : "success") << endl;
    return res;
}

void OrderBookManager::save(Order &new_order) {
    sqlite3_stmt *stmt;
    const string sql = CREATE_ORDER_SQL ORDER_VALUES_SQL;
    int res = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
    if (res) {
        cerr << "failed to prepare statement: " << sqlite3_errstr(res) << endl;
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
        cerr << "error executing statement: " << sqlite3_errstr(res) << endl;
        cerr << "error code " << res << endl;
    } else {
        new_order.id = sqlite3_last_insert_rowid(db);
    }

    sqlite3_finalize(stmt);
}

void OrderBookManager::save_all(vector<Order> &new_orders) {
    int size = new_orders.size();
    string sql = CREATE_ORDER_SQL;
    for (int i = 0; i < size; ++i)
        sql += i == 0 ? ORDER_VALUES_SQL : ORDER_VALUES_CONT_SQL;

    sqlite3_stmt *stmt;
    int res = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
    if (res) {
        cerr << "failed to prepare statement: " << sqlite3_errstr(res) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    for (int i = 0; i < size; ++i) {
        Order &new_order = new_orders[i];
        sqlite3_bind_text(stmt, (i * 5) + 1, new_order.ticker_symbol.c_str(), new_order.ticker_symbol.size(), nullptr);
        sqlite3_bind_text(stmt, (i * 5) + 2, new_order.side.c_str(), new_order.side.size(), nullptr);
        sqlite3_bind_int(stmt, (i * 5) + 3, new_order.amount);
        sqlite3_bind_double(stmt, (i * 5) + 4, new_order.price);
        sqlite3_bind_double(stmt, (i * 5) + 5, new_order.total);
    }

    res = sqlite3_step(stmt);
    if (res != SQLITE_DONE) {
        cerr << "error executing statement: " << sqlite3_errstr(res) << endl;
        cerr << "error code " << res << endl;
    }

    sqlite3_finalize(stmt);
}

void OrderBookManager::cancel_by_id(const int &id) {
    sqlite3_stmt *stmt;
    int res = sqlite3_prepare_v2(db, CANCEL_ORDER_SQL, string(CANCEL_ORDER_SQL).size(), &stmt, nullptr);
    if (res) {
        cerr << "failed to prepare statement: " << sqlite3_errstr(res) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    res = sqlite3_step(stmt);
    if (res != SQLITE_DONE) {
        cerr << "error executing statement: " << sqlite3_errstr(res) << endl;
        cerr << "error code " << res << endl;
    }

    sqlite3_finalize(stmt);
}

OrderBookManager::~OrderBookManager() {
    sqlite3_close(db);
}

vector<Order> OrderBookManager::get_orders_by_symbol(const string &ticker_symbol) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, GET_ORDERS_BY_SYMBOL_SQL, string(GET_ORDERS_BY_SYMBOL_SQL).size(), &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, ticker_symbol.c_str(), ticker_symbol.size(), nullptr);

    vector<Order> results;

    int res;
    res = sqlite3_step(stmt);
    while (res != SQLITE_DONE) {
        int id = sqlite3_column_int(stmt, 0);
        string side{(char *) sqlite3_column_text(stmt, 1)};
        int amount = sqlite3_column_int(stmt, 2);
        double price = sqlite3_column_double(stmt, 3);
        double total = sqlite3_column_double(stmt, 4);

        results.push_back({id, ticker_symbol, side, amount, price, total});

        res = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    return move(results);
}

ostream &operator<<(ostream &o, const Order &order) {
    o << "Order{ " << order.id
      << ", " << order.ticker_symbol
      << ", " << order.side
      << ", " << order.amount
      << ", " << fixed << setprecision(2) << order.price
      << ", " << fixed << setprecision(2) << order.total
      << " }";
    return o;
}