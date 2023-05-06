#include <iostream>
#include "OrderSimulator.h"

#define CMD_MAKE "make"
#define CMD_SHOW "show"
#define CMD_QUIT "q"

using namespace std;

int main() {
    OrderSimulator sim = sp500();
    OrderBookManager orderBookManager;

    vector<Order> orders;
    int n;
    string command, symbol;
    bool done = false;

    while (!done) {
        cout << "Enter command (q to quit): ";
        cin >> command;
        if (command == CMD_MAKE) {
            cin >> n;
            orders = sim.random_orders(n);
            orderBookManager.save_all(orders);

            cout << "CREATED " << orders.size() << " ORDERS\n\n";
        } else if (command == CMD_SHOW) {
            cin >> symbol;
            orders = orderBookManager.get_orders_by_symbol(symbol);
            cout << symbol << " ORDERS\n\n";
            for (const auto &order: orders)
                cout << order << endl;
            cout << endl;
        } else if (command == CMD_QUIT) {
            cout << "Bye!\n\n";
            done = true;
        } else {
            cerr << "Uknown command\n\n";
        }
    }

    return 0;
}
