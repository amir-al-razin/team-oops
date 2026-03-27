#include "Customer.h"
#include "PremiumCustomer.h"

#include <stdexcept>

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

void Customer::upgradeToPremium(Customer*& customer, double loyaltyPercent) {
    if (!customer) {
        throw invalid_argument("Customer pointer is null.");
    }
    if (loyaltyPercent < 0.0 || loyaltyPercent >= 1.0) {
        throw invalid_argument("Loyalty percent must be in [0.0, 1.0).");
    }
    if (dynamic_cast<PremiumCustomer*>(customer)) {
        return;
    }

    auto* upgraded = new PremiumCustomer(customer->id, customer->name, loyaltyPercent);
    for (int orderId : customer->orderHistory) {
        upgraded->addOrderToHistory(orderId);
    }

    delete customer;
    customer = upgraded;
}