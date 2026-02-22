#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include <string>
#include <utility>   // for std::pair

#include "Product.h"
#include "Customer.h"

// Forward declaration to avoid circular include issues
class Finance;

class Order {
private:
    int orderId;
    Customer* customer;   // Polymorphic pointer
    std::vector<std::pair<Product*, int>> items;
    std::string date;
    double totalAmount;
    bool isFinalized;

public:
    // Constructor
    Order(int orderId, Customer* customer, const std::string& date);

    // Core operations
    void addItem(Product* product, int quantity);
    void removeItem(Product* product);
    double calculateTotal();                  // Calculates subtotal (no discount)
    void finalize(Finance& finance);          // Applies discount + updates stock + records revenue
    void setFinalized(bool v);

    // Getters
    int getOrderId() const;
    Customer* getCustomer() const;
    const std::vector<std::pair<Product*, int>>& getItems() const;
    const std::string& getDate() const;
    double getTotalAmount() const;
    bool getIsFinalized() const;

    // Optional helper
    void display() const;
};

#endif