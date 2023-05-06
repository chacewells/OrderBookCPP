//
// Created by chace on 5/5/2023.
//

#include "OrderSimulator.h"

OrderSimulator sp500() {
    std::ifstream infile(STOCKS_FILE, std::ios_base::in);
    std::string symbol;
    std::vector<std::string> symbols;
    while (infile >> symbol) symbols.push_back(symbol);

    return symbols;
}

vector<Order> OrderSimulator::random_orders(int n) {
    vector<Order> orders;

    for (int i = 0; i < n; ++i)
        orders.push_back(random_order());

    return orders;
}

Order OrderSimulator::random_order() {
    int amount = random_amount();
    double price = random_price();
    return {random_symbol(), random_side(), amount, price, amount*price};
}
