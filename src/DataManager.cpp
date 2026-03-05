#include "DataManager.h"
#include "FileManager.h"

#include <stdexcept>
using namespace std;

DataManager::~DataManager() {
    clearCustomers();
}

void DataManager::clearCustomers() {
    for (auto* c : m_customers) delete c;
    m_customers.clear();
}

string DataManager::joinPath(const string& dir, const string& file) {
    if (dir.empty()) return file;
    if (dir.back() == '/' || dir.back() == '\\') return dir + file;
    return dir + "/" + file;
}

void DataManager::loadAll(const string& dataDir) {
    // Clear previous in-memory state
    clearCustomers();
    m_products.clear();
    m_orders.clear();
    m_finance = Finance();

    const string productsFile  = joinPath(dataDir, "products.txt");
    const string customersFile = joinPath(dataDir, "customers.txt");
    const string ordersFile    = joinPath(dataDir, "orders.txt");
    const string financeFile   = joinPath(dataDir, "finance.txt");

    // Load in dependency order:
    m_products  = FileManager::loadProducts(productsFile);
    m_customers = FileManager::loadCustomers(customersFile);
    m_finance   = FileManager::loadFinance(financeFile);

    // Orders need products + customers to resolve pointers
    m_orders = FileManager::loadOrders(ordersFile, m_products, m_customers);
}

void DataManager::saveAll(const string& dataDir) const {
    const string productsFile  = joinPath(dataDir, "products.txt");
    const string customersFile = joinPath(dataDir, "customers.txt");
    const string ordersFile    = joinPath(dataDir, "orders.txt");
    const string financeFile   = joinPath(dataDir, "finance.txt");

    FileManager::saveProducts(m_products, productsFile);
    FileManager::saveCustomers(m_customers, customersFile);
    FileManager::saveOrders(m_orders, ordersFile);
    FileManager::saveFinance(m_finance, financeFile);
}

// Accessors
vector<Product>& DataManager::products() { return m_products; }
vector<Customer*>& DataManager::customers() { return m_customers; }
vector<Order>& DataManager::orders() { return m_orders; }
Finance& DataManager::finance() { return m_finance; }

const vector<Product>& DataManager::products() const { return m_products; }
const vector<Customer*>& DataManager::customers() const { return m_customers; }
const vector<Order>& DataManager::orders() const { return m_orders; }
const Finance& DataManager::finance() const { return m_finance; }