#include "Order.h"
#include <algorithm>
#include <stdexcept>

Order::Order(int orderId, Customer* customer, const std::string& date)
    : orderId(orderId), customer(customer), date(date), totalAmount(0.0), isFinalized(false) {}

int Order::getOrderId() const {
    return orderId;
}

Customer* Order::getCustomer() const {
    return customer;
}

const std::vector<std::pair<Product*, int>>& Order::getItems() const {
    return items;
}

const std::string& Order::getDate() const {
    return date;
}

double Order::getTotalAmount() const {
    return totalAmount;
}

bool Order::getIsFinalized() const {
    return isFinalized;
}

void Order::addItem(Product* product, int qty) {
    if (qty <= 0) {
        throw std::invalid_argument("Quantity must be positive.");
    }
    if (qty > product->getQuantity()) {
        throw std::runtime_error("Insufficient stock for product: " + product->getName());
    }
    // For now, add the item; stock update will happen on finalize
    items.emplace_back(product, qty);
}

void Order::removeItem(Product* product) {
    auto it = std::find_if(items.begin(), items.end(),
        [product](const std::pair<Product*, int>& p) { return p.first == product; });
    if (it != items.end()) {
        items.erase(it);
    }
}

double Order::calculateTotal() {
    totalAmount = 0.0;
    for (const auto& item : items) {
        totalAmount += item.first->getPrice() * item.second;
    }
    if (customer) {
        totalAmount *= (1.0 - customer->calculateDiscount());
    }
    return totalAmount;
}

void Order::finalize() {
    if (isFinalized) {
        throw std::runtime_error("Order is already finalized.");
    }
    calculateTotal();
    isFinalized = true;
    // TODO: Update stock, record revenue, add to customer history (in later phases)
}