//
// Created by chace on 4/25/2023.
//

#ifndef ORDERBOOKCPP_ORDERBOOKMANAGER_H
#define ORDERBOOKCPP_ORDERBOOKMANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>

namespace orderbook {
    struct Order;

    class OrderBookManager {
        sqlite3 *db;
    public:
        OrderBookManager();

        int bootstrap_schema();
        void create(orderbook::Order &new_order);
        void cancel_by_id(const int&);
        std::vector<Order> get_orders_by_symbol(const std::string& ticker_symbol);

        ~OrderBookManager();
    };

    struct Order {
        int id;
        std::string ticker_symbol;
        std::string side;
        int amount;
        double price;
        double total;

        Order(int i, std::string ts, std::string sd, int amt, double pr, double ttl) :
                Order(ts, sd, amt, pr, ttl) {
            id = i;
        }

        Order(std::string ts, std::string sd, int amt, double pr, double ttl) :
                ticker_symbol{ts},
                side{sd},
                amount{amt},
                price{pr},
                total{ttl} {}
    };

    std::ostream& operator<<(std::ostream& o, const Order& order);

} // orderbook

#endif //ORDERBOOKCPP_ORDERBOOKMANAGER_H
