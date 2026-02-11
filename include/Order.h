#ifndef ORDER_H
#define ORDER_H

#include "Customer.h"
#include "Product.h"
#include <vector>
#include <utility>
#include <string>

class Order {
private:
    int orderId;
    Customer* customer;
    std::vector<std::pair<Product*, int>> items;
    std::string date;
    double totalAmount;
    bool isFinalized;

public:
    Order(int orderId, Customer* customer, const std::string& date);
    int getOrderId() const;
    Customer* getCustomer() const;
    const std::vector<std::pair<Product*, int>>& getItems() const;
    const std::string& getDate() const;
    double getTotalAmount() const;
    bool getIsFinalized() const;

    void addItem(Product* product, int qty);
    void removeItem(Product* product);
    double calculateTotal();
    void finalize();
};

#endif