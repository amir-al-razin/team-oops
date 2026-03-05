#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

class Customer {
protected:
    int id;
    std::string name;
    std::vector<int> orderHistory;

public:
    Customer(int id, const std::string& name);
    virtual ~Customer() = default;

    int getId() const;
    const std::string& getName() const;
    const std::vector<int>& getOrderHistory() const;
    void addOrderToHistory(int orderId);

    virtual double calculateDiscount() const = 0;
    void upgradeToPremium(Customer*& customer, double loyaltyPercent);
};

#endif