#include <iostream>
#include <vector>
#include "OrderBookManager.h"

int main() {
    // get sqlite3 handle
    orderbook::OrderBookManager orderBookManager;
    orderBookManager.bootstrap_schema();
    std::vector<std::string> symbols {"TSLA", "AMZN", "MSFT", "AAPL", "FB", "ORCL"};
    std::vector<orderbook::Order> orders_created;

    for (auto &s: symbols) {
        orderbook::Order order (s, "buy", 2, 120, 240);
        orderBookManager.create(order);
        orders_created.push_back(order);
        std::cout << "successfully created " << order << std::endl;

        order = {s, "sell", 2, 120, 240};
        orderBookManager.create(order);
        orders_created.push_back(order);
        std::cout << "successfully created " << order << std::endl;
    }

    std::cout << "TSLA order book:\n";
    for (auto& o : orderBookManager.get_orders_by_symbol("TSLA"))
        std::cout << o << std::endl;
//
//    for (auto itr = orders_created.begin(); itr != orders_created.end(); itr += 2) {
//        orderBookManager.cancel_by_id(itr->id);
//        std::cout << "canceled: " << *itr << std::endl;
//    }

    return 0;
}
