//
// Created by chace on 4/25/2023.
//

#ifndef ORDERBOOKCPP_ORDERBOOKMANAGER_H
#define ORDERBOOKCPP_ORDERBOOKMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

struct Order;

class OrderBookManager {
    sqlite3 *db;
public:
    OrderBookManager();

    int bootstrap_schema();

    void save(Order &new_order);

    void save_all(vector<Order> &new_orders);

    void cancel_by_id(const int &);

    vector<Order> get_orders_by_symbol(const string &ticker_symbol);

    ~OrderBookManager();
};

struct Order {
    int id;
    string ticker_symbol;
    string side;
    int amount;
    double price;
    double total;

    Order(int i, string ts, string sd, int amt, double pr, double ttl) :
            Order(ts, sd, amt, pr, ttl) {
        id = i;
    }

    Order(string ts, string sd, int amt, double pr, double ttl) :
            ticker_symbol{ts},
            side{sd},
            amount{amt},
            price{pr},
            total{ttl} {}
};

ostream &operator<<(ostream &o, const Order &order);

#endif //ORDERBOOKCPP_ORDERBOOKMANAGER_H
