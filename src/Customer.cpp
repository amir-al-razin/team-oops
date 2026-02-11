#include "Customer.h"

Customer::Customer(int id, const std::string& name) : id(id), name(name) {}

int Customer::getId() const {
    return id;
}

const std::string& Customer::getName() const {
    return name;
}

const std::vector<int>& Customer::getOrderHistory() const {
    return orderHistory;
}

void Customer::addOrderToHistory(int orderId) {
    orderHistory.push_back(orderId);
}