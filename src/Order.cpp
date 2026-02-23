#include "Order.h"
#include "Exceptions.h"
#include "Finance.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <utility>

Order::Order(int orderId, Customer* customer, const std::string& date)
    : orderId(orderId),
      customer(customer),
      date(date),
      totalAmount(0.0),
      isFinalized(false) {}

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
    if (!product) {
        throw InvalidInputException("Product is null.");
    }
    if (qty <= 0) {
        throw InvalidInputException("Quantity must be positive.");
    }
    if (qty > product->getQuantity()) {
        throw InsufficientStockException("Insufficient stock for product: " + product->getName());
    }
    if (isFinalized) {
        throw InvalidInputException("Cannot modify a finalized order.");
    }

    // Optional improvement: if same product already exists in cart, merge qty
    for (auto& it : items) {
        if (it.first == product) {
            int newQty = it.second + qty;
            if (newQty > product->getQuantity()) {
                throw InsufficientStockException("Insufficient stock to increase quantity for product: " + product->getName());
            }
            it.second = newQty;
            return;
        }
    }

    items.emplace_back(product, qty);
}

void Order::removeItem(Product* product) {
    if (isFinalized) {
        throw InvalidInputException("Cannot modify a finalized order.");
    }

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

    return totalAmount;
}


void Order::finalize(Finance& finance) {
    if (isFinalized) {
        throw InvalidInputException("Order already finalized.");
    }
    if (!customer) {
        throw InvalidInputException("Order has no customer.");
    }
    if (items.empty()) {
        throw InvalidInputException("Cannot finalize an empty order.");
    }

    // Step 1: Calculate subtotal
    calculateTotal();

    // Step 2: Apply discount
    double discount = customer->calculateDiscount();
    if (discount < 0.0 || discount >= 1.0) {
        throw InvalidInputException("Invalid discount value.");
    }
    totalAmount = totalAmount * (1.0 - discount);

    // Step 3: Update stock (reduce inventory)
    for (auto& item : items) {
        Product* product = item.first;
        int qty = item.second;

        // updateStock should throw if it goes negative
        product->updateStock(-qty);
    }

    // Step 4: Record revenue
    finance.recordRevenue(
        totalAmount,
        "Order #" + std::to_string(orderId),
        date
    );

    // Step 5: Customer order history
    customer->addOrderToHistory(orderId);

    // Step 6: Lock the order
    isFinalized = true;
}

void Order::setFinalized(bool v) {
    isFinalized = v;
}

void Order::printInvoice() const {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    cout << "\n============================================================\n";
    cout << "                     INVOICE / RECEIPT\n";
    cout << "============================================================\n";

    cout << "Order ID   : " << orderId << "\n";
    cout << "Date       : " << date << "\n";
    cout << "Customer   : " << (customer ? customer->getName() : "N/A") << "\n";
    cout << "CustomerID : " << (customer ? std::to_string(customer->getId()) : "N/A") << "\n";
    cout << "Status     : " << (isFinalized ? "FINALIZED" : "PENDING") << "\n";

    cout << "------------------------------------------------------------\n";
    cout << left
         << setw(6)  << "Qty"
         << setw(22) << "Item"
         << right
         << setw(12) << "Unit"
         << setw(12) << "Line"
         << "\n";
    cout << "------------------------------------------------------------\n";

    cout << std::fixed << std::setprecision(2);

    double subtotal = 0.0;
    for (const auto& it : items) {
        Product* p = it.first;
        int qty = it.second;
        if (!p) continue;

        double unit = p->getPrice();
        double line = unit * qty;
        subtotal += line;

        cout << left
             << setw(6)  << qty
             << setw(22) << p->getName().substr(0, 21)   // prevent overflow
             << right
             << setw(12) << unit
             << setw(12) << line
             << "\n";
    }

    cout << "------------------------------------------------------------\n";

    // totalAmount is assumed to be final total AFTER discount (from finalize)
    double total = totalAmount;
    double discountAmount = subtotal - total;

    // If order not finalized yet, totalAmount might just be subtotal
    if (!isFinalized) {
        total = subtotal;
        discountAmount = 0.0;
    }

    cout << left << setw(28) << "Subtotal:"
         << right << setw(24) << subtotal << "\n";

    cout << left << setw(28) << "Discount:"
         << right << setw(24) << discountAmount << "\n";

    cout << left << setw(28) << "TOTAL PAYABLE:"
         << right << setw(24) << total << "\n";

    cout << "============================================================\n\n";
}