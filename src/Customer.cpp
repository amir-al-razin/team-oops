#include "Customer.h"
using namespace std;

Customer::Customer(int id, const std::string& name) : id(id), name(name) {}

int Customer::getId() const {
    return id;
}

const string& Customer::getName() const {
    return name;
}

const vector<int>& Customer::getOrderHistory() const {
    return orderHistory;
}

void Customer::addOrderToHistory(int orderId) {
    orderHistory.push_back(orderId);
}