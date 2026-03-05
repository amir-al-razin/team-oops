#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <string>
#include <vector>

#include "Product.h"
#include "Customer.h"
#include "Order.h"
#include "Finance.h"

class DataManager {
public:
    DataManager() = default;
    ~DataManager(); // important: free Customer* pointers

    // Load/Save everything
    void loadAll(const std::string& dataDir = "data");
    void saveAll(const std::string& dataDir = "data") const;

    // Accessors
    std::vector<Product>& products();
    std::vector<Customer*>& customers();
    std::vector<Order>& orders();
    Finance& finance();

    const std::vector<Product>& products() const;
    const std::vector<Customer*>& customers() const;
    const std::vector<Order>& orders() const;
    const Finance& finance() const;

private:
    std::vector<Product> m_products;
    std::vector<Customer*> m_customers;  // polymorphic ownership (manual delete in destructor)
    std::vector<Order> m_orders;
    Finance m_finance;

    static std::string joinPath(const std::string& dir, const std::string& file);
    void clearCustomers();
};

#endif