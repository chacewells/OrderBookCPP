#include <iostream>
#include "OrderBookManager.h"

int main() {
    // get sqlite3 handle
    orderbook::OrderBookManager orderBookManager;
    orderBookManager.bootstrap_schema();
    orderbook::Order order ("TSLA", "BUY", 2, 120.0, 240.0);
    orderBookManager.create(order);

    std::cout << "successfully created " << order << std::endl;
    return 0;
}
