#include "FileManager.h"
#include "Exceptions.h"
#include "RegularCustomer.h"
#include "PremiumCustomer.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

// ---------------- Helpers ----------------

std::vector<std::string> FileManager::split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
    return out;
}

std::string FileManager::trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) b++;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) e--;
    return s.substr(b, e - b);
}

Product* FileManager::findProductById(std::vector<Product>& products, int id) {
    for (auto& p : products) {
        if (p.getId() == id) return &p;
    }
    return nullptr;
}

Customer* FileManager::findCustomerById(std::vector<Customer*>& customers, int id) {
    for (auto* c : customers) {
        if (c && c->getId() == id) return c;
    }
    return nullptr;
}

// ---------------- Products ----------------

std::vector<Product> FileManager::loadProducts(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open products file: " + filepath);

    std::vector<Product> products;
    std::string line;

    // optional header
    if (std::getline(in, line)) {
        if (line.find("ID") == std::string::npos) {
            // first line is data, process it
            in.seekg(0);
        }
    } else {
        return products; // empty file
    }

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 5) throw FileOperationException("Invalid products line: " + line);

        int id = std::stoi(trim(cols[0]));
        std::string name = trim(cols[1]);
        double price = std::stod(trim(cols[2]));
        double cost = std::stod(trim(cols[3]));
        int qty = std::stoi(trim(cols[4]));

        products.emplace_back(id, name, price, cost, qty);
    }

    return products;
}

void FileManager::saveProducts(const std::vector<Product>& products, const std::string& filepath) {
    std::ofstream out(filepath);
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

std::vector<Customer*> FileManager::loadCustomers(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open customers file: " + filepath);

    std::vector<Customer*> customers;
    std::string line;

    // optional header
    if (std::getline(in, line)) {
        if (line.find("Type") == std::string::npos) {
            in.seekg(0);
        }
    } else {
        return customers;
    }

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 4) throw FileOperationException("Invalid customers line: " + line);

        int id = std::stoi(trim(cols[0]));
        std::string name = trim(cols[1]);
        std::string type = trim(cols[2]);
        double loyalty = std::stod(trim(cols[3]));

        Customer* c = nullptr;
        if (type == "Premium") {
            c = new PremiumCustomer(id, name, loyalty);
        } else if (type == "Regular") {
            c = new RegularCustomer(id, name);
        } else {
            throw FileOperationException("Unknown customer type: " + type);
        }

        // order history (optional col 5)
        if (cols.size() >= 5) {
            std::string hist = trim(cols[4]);
            if (!hist.empty()) {
                auto ids = split(hist, ';');
                for (const auto& sId : ids) {
                    std::string t = trim(sId);
                    if (!t.empty()) c->addOrderToHistory(std::stoi(t));
                }
            }
        }

        customers.push_back(c);
    }

    return customers;
}

void FileManager::saveCustomers(const std::vector<Customer*>& customers, const std::string& filepath) {
    std::ofstream out(filepath);
    if (!out.is_open()) throw FileOperationException("Failed to write customers file: " + filepath);

    out << "ID,Name,Type,LoyaltyPercentage,OrderIDs\n";

    for (auto* c : customers) {
        if (!c) continue;

        std::string type = "Regular";
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
//
// Important: We reconstruct Order items by linking pointers to loaded products/customers.

std::vector<Order> FileManager::loadOrders(const std::string& filepath,
                                          std::vector<Product>& products,
                                          std::vector<Customer*>& customers) {
    std::ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open orders file: " + filepath);

    std::vector<Order> orders;
    std::string line;

    // optional header
    if (std::getline(in, line)) {
        if (line.find("OrderID") == std::string::npos) {
            in.seekg(0);
        }
    } else {
        return orders;
    }

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 6) throw FileOperationException("Invalid orders line: " + line);

        int orderId = std::stoi(trim(cols[0]));
        int customerId = std::stoi(trim(cols[1]));
        std::string date = trim(cols[2]);
        // cols[3] totalAmount (we can ignore, it can be recalculated)
        std::string finalizedStr = trim(cols[4]);
        std::string itemsStr = trim(cols[5]);

        Customer* c = findCustomerById(customers, customerId);
        if (!c) throw FileOperationException("Order references missing customerId: " + std::to_string(customerId));

        Order o(orderId, c, date);

        if (!itemsStr.empty()) {
            auto itemPairs = split(itemsStr, ';');
            for (const auto& ip : itemPairs) {
                auto parts = split(trim(ip), ':');
                if (parts.size() != 2) throw FileOperationException("Invalid order item: " + ip);

                int pid = std::stoi(trim(parts[0]));
                int qty = std::stoi(trim(parts[1]));

                Product* p = findProductById(products, pid);
                if (!p) throw FileOperationException("Order references missing productId: " + std::to_string(pid));

                // addItem validates stock; for historical load we still want the structure.
                // If your saved orders can exceed current stock, you can bypass addItem by temporarily
                // disabling that check. For now we keep addItem.
                o.addItem(p, qty);
            }
        }

        o.calculateTotal();

        // If it was finalized when saved, finalize it again (only toggles bool in your current code)
        // Your finalize() currently doesn't update stock; that's fine for loading.
        if (finalizedStr == "true" || finalizedStr == "1" || finalizedStr == "Yes") {
            o.finalize();
        }

        orders.push_back(o);
    }

    return orders;
}

void FileManager::saveOrders(const std::vector<Order>& orders, const std::string& filepath) {
    std::ofstream out(filepath);
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

Finance FileManager::loadFinance(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) throw FileOperationException("Failed to open finance file: " + filepath);

    Finance f;
    std::string line;

    // header
    if (!std::getline(in, line)) return f;

    // totals
    if (!std::getline(in, line)) return f;
    line = trim(line);
    if (!line.empty()) {
        auto cols = split(line, ',');
        if (cols.size() >= 2) {
            double rev = std::stod(trim(cols[0]));
            double exp = std::stod(trim(cols[1]));

            // set by recording transactions so totals match
            if (rev > 0) f.recordRevenue(rev, "Loaded total revenue", "N/A");
            if (exp > 0) f.recordExpense(exp, "Loaded total expenses", "N/A");
        }
    }

    // optional transaction header
    std::getline(in, line);

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        auto cols = split(line, ',');
        if (cols.size() < 4) throw FileOperationException("Invalid finance transaction line: " + line);

        std::string type = trim(cols[0]);
        double amount = std::stod(trim(cols[1]));
        std::string date = trim(cols[2]);
        std::string desc = trim(cols[3]);

        if (type == "Revenue") f.recordRevenue(amount, desc, date);
        else if (type == "Expense") f.recordExpense(amount, desc, date);
        else throw FileOperationException("Unknown transaction type: " + type);
    }

    return f;
}

void FileManager::saveFinance(const Finance& finance, const std::string& filepath) {
    std::ofstream out(filepath);
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