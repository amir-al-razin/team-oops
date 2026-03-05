#include "DataManager.h"
#include "FileManager.h"

#include <stdexcept>

DataManager::~DataManager() {
    clearCustomers();
}

void DataManager::clearCustomers() {
    for (auto* c : m_customers) delete c;
    m_customers.clear();
}

std::string DataManager::joinPath(const std::string& dir, const std::string& file) {
    if (dir.empty()) return file;
    if (dir.back() == '/' || dir.back() == '\\') return dir + file;
    return dir + "/" + file;
}

void DataManager::loadAll(const std::string& dataDir) {
    // Clear previous in-memory state
    clearCustomers();
    m_products.clear();
    m_orders.clear();
    m_finance = Finance();

    const std::string productsFile  = joinPath(dataDir, "products.txt");
    const std::string customersFile = joinPath(dataDir, "customers.txt");
    const std::string ordersFile    = joinPath(dataDir, "orders.txt");
    const std::string financeFile   = joinPath(dataDir, "finance.txt");

    // Load in dependency order:
    m_products  = FileManager::loadProducts(productsFile);
    m_customers = FileManager::loadCustomers(customersFile);
    m_finance   = FileManager::loadFinance(financeFile);

    // Orders need products + customers to resolve pointers
    m_orders = FileManager::loadOrders(ordersFile, m_products, m_customers);
}

void DataManager::saveAll(const std::string& dataDir) const {
    const std::string productsFile  = joinPath(dataDir, "products.txt");
    const std::string customersFile = joinPath(dataDir, "customers.txt");
    const std::string ordersFile    = joinPath(dataDir, "orders.txt");
    const std::string financeFile   = joinPath(dataDir, "finance.txt");

    FileManager::saveProducts(m_products, productsFile);
    FileManager::saveCustomers(m_customers, customersFile);
    FileManager::saveOrders(m_orders, ordersFile);
    FileManager::saveFinance(m_finance, financeFile);
}

// Accessors
std::vector<Product>& DataManager::products() { return m_products; }
std::vector<Customer*>& DataManager::customers() { return m_customers; }
std::vector<Order>& DataManager::orders() { return m_orders; }
Finance& DataManager::finance() { return m_finance; }

const std::vector<Product>& DataManager::products() const { return m_products; }
const std::vector<Customer*>& DataManager::customers() const { return m_customers; }
const std::vector<Order>& DataManager::orders() const { return m_orders; }
const Finance& DataManager::finance() const { return m_finance; }