#include "FileManager.h"
#include "Exceptions.h"
#include "RegularCustomer.h"
#include "PremiumCustomer.h"
#include "Finance.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
using namespace std;

// ---------------- Helpers ----------------

vector<string> FileManager::split(const string& s, char delim) {
    vector<string> out;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) out.push_back(item);
    return out;
}

string FileManager::trim(const string& s) {
    size_t b = 0, e = s.size();
    while (b < e && isspace(static_cast<unsigned char>(s[b]))) b++;
    while (e > b && isspace(static_cast<unsigned char>(s[e - 1]))) e--;
    return s.substr(b, e - b);
}

Product* FileManager::findProductById(vector<Product>& products, int id) {
    for (auto& p : products) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

Customer* FileManager::findCustomerById(vector<Customer*>& customers, int id) {
    for (auto* c : customers) {
        if (c && c->getId() == id) return c;
    }
    return nullptr;
}

// ---------------- Products ----------------

vector<Product> FileManager::loadProducts(const string& filepath) {
    ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open products file: " + filepath);

    vector<Product> products;
    string line;

    // optional header
    if (getline(in, line)) {
        if (line.find("ID") == string::npos) {
            // first line is data, process it
            in.seekg(0);
        }
    } else {
        return products; // empty file
    }

    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 5) throw FileOperationException("Invalid products line: " + line);

        int id = stoi(trim(cols[0]));
        string name = trim(cols[1]);
        double price = stod(trim(cols[2]));
        double cost = stod(trim(cols[3]));
        int qty = stoi(trim(cols[4]));

        products.emplace_back(id, name, price, cost, qty);
    }

    return products;
}

void FileManager::saveProducts(const vector<Product>& products, const string& filepath) {
    ofstream out(filepath);
    if (!out.is_open()) throw FileOperationException("Failed to write products file: " + filepath);

    out << "ID,Name,Price,Cost,Quantity\n";
    for (const auto& p : products) {
        out << p.getId() << ","
            << p.getName() << ","
            << p.getPrice() << ","
            << p.getCost() << ","
            << p.getQuantity() << "\n";
    }
}

// ---------------- Customers ----------------
// Format:
// ID,Name,Type,LoyaltyPercentage,OrderIDs(semi-colon separated)
// Example:
// 101,Alice,Regular,0,1;2
// 102,Bob,Premium,10,3

vector<Customer*> FileManager::loadCustomers(const string& filepath) {
    ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open customers file: " + filepath);

    vector<Customer*> customers;
    string line;

    if (getline(in, line)) {
        if (line.find("Type") == string::npos) {
            in.clear();
            in.seekg(0);
        }
    } else {
        return customers;
    }

    try {
        while (getline(in, line)) {
            line = trim(line);
            if (line.empty()) continue;

            auto cols = split(line, ',');
            if (cols.size() < 4) throw FileOperationException("Invalid customers line: " + line);

            int id = stoi(trim(cols[0]));
            string name = trim(cols[1]);
            string type = trim(cols[2]);
            double loyalty = stod(trim(cols[3]));

            Customer* c = nullptr;
            if (type == "Premium") {
                c = new PremiumCustomer(id, name, loyalty);
            } else if (type == "Regular") {
                c = new RegularCustomer(id, name);
            } else {
                throw FileOperationException("Unknown customer type: " + type);
            }
            if (cols.size() >= 5) {
                string hist = trim(cols[4]);
                if (!hist.empty()) {
                    auto ids = split(hist, ';');
                    for (const auto& sId : ids) {
                        string t = trim(sId);
                        if (!t.empty()) c->addOrderToHistory(stoi(t));
                    }
                }
            }
            
            customers.push_back(c); 
        }
    } catch (...) {
        for (auto* c : customers) delete c;
        throw;
    }

    return customers;
}

void FileManager::saveCustomers(const vector<Customer*>& customers, const string& filepath) {
    ofstream out(filepath);
    if (!out.is_open()) throw FileOperationException("Failed to write customers file: " + filepath);

    out << "ID,Name,Type,LoyaltyPercentage,OrderIDs\n";

    for (auto* c : customers) {
        if (!c) continue;

        string type = "Regular";
        double loyalty = 0.0;

        // detect premium via dynamic_cast
        if (auto* p = dynamic_cast<PremiumCustomer*>(c)) {
            type = "Premium";
            loyalty = p->getLoyaltyPercentage();
        }

        out << c->getId() << ","
            << c->getName() << ","
            << type << ","
            << loyalty << ",";

        const auto& hist = c->getOrderHistory();
        for (size_t i = 0; i < hist.size(); i++) {
            out << hist[i];
            if (i + 1 < hist.size()) out << ";";
        }
        out << "\n";
    }
}

// ---------------- Orders ----------------
// Format:
// OrderID,CustomerID,Date,TotalAmount,Finalized,Items
// Items: product_id:qty;product_id:qty
vector<Order>FileManager::loadOrders(const string& filepath,vector<Product> &products,vector<Customer*>& customers){
    std::ifstream in(filepath);
    if(!in.is_open()){
        throw FileOperationException("Failed to open orders file: " + filepath);
    }
    vector<Order> orders;
    string line;

    // Optional header: clear stream errors before seeking to prevent EOF bugs
    if (std::getline(in, line)) {
        if (line.find("OrderID") == std::string::npos) {
            in.clear(); 
            in.seekg(0);
        }
    } else {
        return orders;
    }

    while(getline(in,line)){
        line = trim(line);
        if(line.empty()){
            continue;
        }
        if(line.find("OrderID") != string::npos){
            continue;
        }
        auto cols = split(line,',');
        // Change to < 5, because an empty trailing item list makes the size 5
        if(cols.size() < 5){
            throw FileOperationException("Invalid orders line: " + line);
        }
        int orderId = stoi(trim(cols[0]));
        int customerId = stoi(trim(cols[1]));
        string date = trim(cols[2]);
        double historicalTotal = stod(trim(cols[3]));
        string finalizedStr = trim(cols[4]);
        string itemsStr = (cols.size() >= 6) ? trim(cols[5]) : "";

        Customer* c = nullptr;
        if(customerId!=-1){
            c = findCustomerById(customers,customerId);
            if(!c){
                throw FileOperationException("Order references missing customerId: " + std::to_string(customerId));
            }

        }

        Order o(orderId, c, date);
        if(!itemsStr.empty()){
            auto itemPairs = split(itemsStr, ';');
            for(const auto& ip : itemPairs){
                string trimmedIp = trim(ip);
                if (trimmedIp.empty()) continue;
            
                auto parts = split(trimmedIp, ':');
                if (parts.size() != 2) {
                    throw FileOperationException("Invalid order item: " + trimmedIp);
                }
                
                int pid = stoi(trim(parts[0]));
                int qty = stoi(trim(parts[1]));

                Product* p = nullptr;
                if (pid != -1) {
                    p = findProductById(products, pid);
                    if (!p) {
                        throw FileOperationException("Order references missing productId: " + to_string(pid));
                    }

                    // Rebuild historical items without validating against current stock.
                    o.addLoadedItem(p, qty);
                }
            }
        }
        o.setTotalAmount(historicalTotal);
        bool finalized = (finalizedStr == "true" || finalizedStr == "1" ||
                          finalizedStr == "Yes"  || finalizedStr == "yes");

        o.setFinalized(finalized);

        orders.push_back(o);

    }
    return orders;
}
void FileManager::saveOrders(const vector<Order>& orders, const string& filepath) {
    ofstream out(filepath);
    if (!out.is_open()) throw FileOperationException("Failed to write orders file: " + filepath);

    out << "OrderID,CustomerID,Date,TotalAmount,Finalized,Items\n";
    for (const auto& o : orders) {
        int customerId = o.getCustomer() ? o.getCustomer()->getId() : -1;

        out << o.getOrderId() << ","
            << customerId << ","
            << o.getDate() << ","
            << o.getTotalAmount() << ","
            << (o.getIsFinalized() ? "true" : "false") << ",";

        const auto& items = o.getItems();
        for (size_t i = 0; i < items.size(); i++) {
            const auto& [p, qty] = items[i];
            out << (p ? p->getId() : -1) << ":" << qty;
            if (i + 1 < items.size()) out << ";";
        }
        out << "\n";
    }
}

// ---------------- Finance ----------------
// Format:
// TotalRevenue,TotalExpenses
// revenue,expenses
// TransactionType,Amount,Date,Description
// ...

Finance FileManager::loadFinance(const string& filepath) {
    ifstream in(filepath);
    if (!in.is_open()) {
        throw FileOperationException("Failed to open finance file: " + filepath);
    }

    Finance f;
    string line;

    // 1) Header: "TotalRevenue,TotalExpenses"
    if (!getline(in, line)) return f;

    // 2) Totals line: "300,204800"
    if (!getline(in, line)) return f;

    // 3) Transaction header: "TransactionType,Amount,Date,Description"
    if (!getline(in, line)) return f;

    // 4) Transactions
    while (getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 4) {
            throw FileOperationException("Invalid finance transaction line: " + line);
        }

        string type = trim(cols[0]);
        string amountStr = trim(cols[1]);
        string date = trim(cols[2]);
        
        string desc = cols[3];
        for (size_t i = 4; i < cols.size(); ++i) {
            desc += "," + cols[i];
        }
        desc = trim(desc);

        if (desc.find("Loaded total") != string::npos) continue;

        double amount;
        try {
            amount = stod(amountStr);
        } catch (...) {
            throw FileOperationException("Invalid amount '" + amountStr + "' in line: " + line);
        }

        if (type == "Revenue") {
            f.recordRevenue(amount, desc, date);
        } else if (type == "Expense") {
            f.recordExpense(amount, desc, date);
        } else {
            throw FileOperationException("Unknown transaction type: " + type);
        }
    }

    return f;
}
void FileManager::saveFinance(const Finance& finance, const string& filepath) {
    ofstream out(filepath);
    if (!out.is_open()) throw FileOperationException("Failed to write finance file: " + filepath);

    out << "TotalRevenue,TotalExpenses\n";
    out << finance.getTotalRevenue() << "," << finance.getTotalExpenses() << "\n";
    out << "TransactionType,Amount,Date,Description\n";

    for (const auto& t : finance.getTransactions()) {
        out << t.type << ","
            << t.amount << ","
            << t.date << ","
            << t.description << "\n";
    }
}
