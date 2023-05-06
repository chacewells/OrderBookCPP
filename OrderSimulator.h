//
// Created by chace on 5/5/2023.
//

#ifndef ORDERBOOKCPP_ORDERSIMULATOR_H
#define ORDERBOOKCPP_ORDERSIMULATOR_H

#define STOCKS_FILE "sp500.txt"


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include "OrderBookManager.h"

using namespace std;

struct OrderSimulator {
    OrderSimulator(vector<string> stonks) :
            sp_stocks(stonks),
            stock_rand(0, sp_stocks.size() - 1),
            side_rand(false, true),
            amount_rand(0, 511),
            price_rand(0.0, 1000.0),
            gen(rd()) {}

    Order random_order();

    vector<Order> random_orders(int n);

private:
    vector<string> sp_stocks;
    random_device rd;
    uniform_int_distribution<> stock_rand;
    uniform_int_distribution<unsigned char> side_rand;
    uniform_int_distribution<> amount_rand;
    uniform_real_distribution<> price_rand;
    mt19937 gen;

    string random_symbol() {
        return sp_stocks[stock_rand(gen)];
    }

    int random_amount() {
        return amount_rand(gen);
    }

    double random_price() {
        return price_rand(gen);
    }

    string random_side() {
        return side_rand(gen) ? "buy" : "sell";
    }
};

OrderSimulator sp500();

#endif //ORDERBOOKCPP_ORDERSIMULATOR_H
