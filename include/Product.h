#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>

class Product {
public:
    Product();
    Product(int id, const std::string &name, double price, double cost, int quantity);

    int getId() const;
    const std::string &getName() const;
    double getPrice() const;
    double getCost() const;
    int getQuantity() const;

    void setId(int id);
    void setName(const std::string &name);
    void setPrice(double price);
    void setCost(double cost);
    void setQuantity(int quantity);

    void updateStock(int qty);
    double calculateProfit() const;
    std::string toString() const;

    // Comparison operators
    bool operator>(const Product& other) const;
    bool operator<(const Product& other) const;
    bool operator==(const Product& other) const;

    // Output operator
    friend std::ostream& operator<<(std::ostream& os, const Product& product);

private:
    int id;
    std::string name;
    double price;
    double cost;
    int quantity;
};

#endif
