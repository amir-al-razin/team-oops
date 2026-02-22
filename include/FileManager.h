#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

#include "Product.h"
#include "Customer.h"
#include "Order.h"
#include "Finance.h"

class FileManager {
public:
    // Products
    static std::vector<Product> loadProducts(const std::string& filepath);
    static void saveProducts(const std::vector<Product>& products, const std::string& filepath);

    // Customers (polymorphic)
    static std::vector<Customer*> loadCustomers(const std::string& filepath);
    static void saveCustomers(const std::vector<Customer*>& customers, const std::string& filepath);

    // Orders
    static std::vector<Order> loadOrders(const std::string& filepath,
                                        std::vector<Product>& products,
                                        std::vector<Customer*>& customers);
    static void saveOrders(const std::vector<Order>& orders, const std::string& filepath);

    // Finance
    static Finance loadFinance(const std::string& filepath);
    static void saveFinance(const Finance& finance, const std::string& filepath);

private:
    // Helpers
    static std::vector<std::string> split(const std::string& s, char delim);
    static std::string trim(const std::string& s);

    static Product* findProductById(std::vector<Product>& products, int id);
    static Customer* findCustomerById(std::vector<Customer*>& customers, int id);
};

#endif