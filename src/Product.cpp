#include "Product.h"

#include <sstream>
#include <stdexcept>

Product::Product() : id(0), name(""), price(0.0), cost(0.0), quantity(0) {}

Product::Product(int id, const std::string &name, double price, double cost, int quantity)
    : id(0), name(""), price(0.0), cost(0.0), quantity(0) {
    setId(id);
    setName(name);
    setPrice(price);
    setCost(cost);
    setQuantity(quantity);
}

int Product::getId() const {
    return id;
}

const std::string &Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

double Product::getCost() const {
    return cost;
}

int Product::getQuantity() const {
    return quantity;
}

void Product::setId(int newId) {
    if (newId <= 0) {
        throw std::invalid_argument("Product id must be positive.");
    }
    id = newId;
}

void Product::setName(const std::string &newName) {
    if (newName.empty()) {
        throw std::invalid_argument("Product name cannot be empty.");
    }
    name = newName;
}

void Product::setPrice(double newPrice) {
    if (newPrice < 0.0) {
        throw std::invalid_argument("Product price cannot be negative.");
    }
    price = newPrice;
}

void Product::setCost(double newCost) {
    if (newCost < 0.0) {
        throw std::invalid_argument("Product cost cannot be negative.");
    }
    cost = newCost;
}

void Product::setQuantity(int newQuantity) {
    if (newQuantity < 0) {
        throw std::invalid_argument("Product quantity cannot be negative.");
    }
    quantity = newQuantity;
}

void Product::updateStock(int qty) {
    if (quantity + qty < 0) {
        throw std::invalid_argument("Stock update would make quantity negative.");
    }
    quantity += qty;
}

double Product::calculateProfit() const {
    return (price - cost) * static_cast<double>(quantity);
}

std::string Product::toString() const {
    std::ostringstream out;
    out << "Product{id=" << id << ", name=\"" << name << "\", price=" << price
        << ", cost=" << cost << ", quantity=" << quantity << "}";
    return out.str();
}

// Comparison operators
bool Product::operator>(const Product& other) const {
    return price > other.price;
}

bool Product::operator<(const Product& other) const {
    return price < other.price;
}

bool Product::operator==(const Product& other) const {
    return id == other.id;
}

// Output operator
std::ostream& operator<<(std::ostream& os, const Product& product) {
    os << "Product{id=" << product.id << ", name=\"" << product.name << "\", price=" << product.price
       << ", cost=" << product.cost << ", quantity=" << product.quantity << "}";
    return os;
}
