#include "MenuSystem.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>

#include "Exceptions.h"
#include "RegularCustomer.h"
#include "PremiumCustomer.h"
#include "Order.h"
MenuSystem::MenuSystem(DataManager& dm) : dm(dm) {}

void MenuSystem::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int MenuSystem::readInt(const char* prompt) {
    while (true) {
        std::cout << prompt;
        int v;
        if (std::cin >> v) {
            clearInput();
            return v;
        }
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

double MenuSystem::readDouble(const char* prompt) {
    while (true) {
        std::cout << prompt;
        double v;
        if (std::cin >> v) {
            clearInput();
            return v;
        }
        std::cout << "Invalid number. Try again.\n";
        clearInput();
    }
}

std::string MenuSystem::readLine(const char* prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void MenuSystem::mainLoop() {
    while (true) {
        std::cout << "\n===== Business Management System =====\n"
                  << "1. Inventory Management\n"
                  << "2. Customer Management\n"
                  << "3. Order Management\n"
                  << "4. Finance Reports\n"
                  << "5. System Reports\n"
                  << "0. Exit\n";

        int choice = readInt("Select: ");

        try {
            switch (choice) {
                case 1: inventoryMenu(); break;
                case 2: customerMenu(); break;
                case 3: orderMenu(); break;
                case 4: financeMenu(); break;
                case 5: reportsMenu(); break;
                case 0: return;
                default: std::cout << "Invalid choice.\n"; break;
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------------- Inventory Menu ----------------

void MenuSystem::inventoryMenu() {
    while (true) {
        std::cout << "\n--- Inventory Menu ---\n"
                  << "1. Add Product\n"
                  << "2. Restock Product\n"
                  << "3. List Products\n"
                  << "4. Low Stock Alert\n"
                  << "0. Back\n";

        int choice = readInt("Select: ");

        switch (choice) {
            case 1: addProduct(); dm.saveAll("data"); break;
            case 2: restockProduct(); dm.saveAll("data"); break;
            case 3: listProducts(); break;
            case 4: lowStockAlert(); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }
}

void MenuSystem::addProduct() {
    int id = readInt("Product ID: ");
    std::string name = readLine("Name: ");
    double price = readDouble("Selling Price: ");
    double cost = readDouble("Cost Price: ");
    int qty = readInt("Initial Quantity: ");

    dm.products().emplace_back(id, name, price, cost, qty);

    // Step 3 (optional): initial stock purchase expense
    // dm.finance().recordExpense(cost * qty, "Initial stock for " + name, "N/A");

    std::cout << "Product added.\n";
}

void MenuSystem::restockProduct() {
    int id = readInt("Product ID to restock: ");
    int qty = readInt("Quantity to add: ");

    for (auto& p : dm.products()) {
        if (p.getId() == id) {
            p.updateStock(qty);

            //Step 3: record expense for buying stock (recommended)
            dm.finance().recordExpense(p.getCost() * qty,
                                      "Restock product #" + std::to_string(id),
                                      "N/A");

            std::cout << "Restocked.\n";
            return;
        }
    }

    throw InvalidInputException("Product ID not found.");
}

void MenuSystem::listProducts() {
    using std::cout;
    using std::left;
    using std::setw;

    cout << "\n";
    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(12) << "Price"
         << setw(12) << "Cost"
         << setw(8)  << "Qty"
         << "\n";

    cout << "-------------------------------------------------------------\n";

    for (const auto& p : dm.products()) {
        cout << left
             << setw(6)  << p.getId()
             << setw(20) << p.getName()
             << setw(12) << p.getPrice()
             << setw(12) << p.getCost()
             << setw(8)  << p.getQuantity()
             << "\n";
    }
}

void MenuSystem::lowStockAlert() {
    int threshold = readInt("Low stock threshold: ");
    bool any = false;

    for (const auto& p : dm.products()) {
        if (p.getQuantity() < threshold) {
            any = true;
            std::cout << "LOW STOCK -> ID " << p.getId()
                      << " (" << p.getName() << ") Qty: " << p.getQuantity() << "\n";
        }
    }

    if (!any) std::cout << "No low-stock items.\n";
}

// ---------------- Customer Menu ----------------

void MenuSystem::customerMenu() {
    while (true) {
        std::cout << "\n--- Customer Menu ---\n"
                  << "1. Add Regular Customer\n"
                  << "2. Add Premium Customer\n"
                  << "3. List Customers\n"
                  << "4. Upgrade Regular -> Premium\n"
                  << "0. Back\n";

        int choice = readInt("Select: ");

        switch (choice) {
            case 1: addRegularCustomer(); dm.saveAll("data"); break;
            case 2: addPremiumCustomer(); dm.saveAll("data"); break;
            case 3: listCustomers(); break;
            case 4: upgradeCustomerToPremium(); dm.saveAll("data"); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }
}

void MenuSystem::addRegularCustomer() {
    int id = readInt("Customer ID: ");
    std::string name = readLine("Name: ");

    dm.customers().push_back(new RegularCustomer(id, name));
    std::cout << "Regular customer added.\n";
}

void MenuSystem::addPremiumCustomer() {
    int id = readInt("Customer ID: ");
    std::string name = readLine("Name: ");
    double loyalty = readDouble("Loyalty (fraction like 0.10 for 10%): ");

    dm.customers().push_back(new PremiumCustomer(id, name, loyalty));
    std::cout << "Premium customer added.\n";
}

void MenuSystem::listCustomers() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    cout << "\n";
    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(12) << "Type"
         << right
         << setw(10) << "Orders"
         << setw(15) << "TotalSpent"
         << "\n";

    cout << "---------------------------------------------------------------\n";
    cout << std::fixed << std::setprecision(2);

    for (auto* c : dm.customers()) {
        if (!c) continue;

        std::string type = dynamic_cast<PremiumCustomer*>(c) ? "Premium" : "Regular";

        double spent = 0.0;
        for (const auto& o : dm.orders()) {
            if (!o.getIsFinalized()) continue;
            if (!o.getCustomer()) continue;
            if (o.getCustomer()->getId() == c->getId()) {
                spent += o.getTotalAmount();
            }
        }

        cout << left
             << setw(6)  << c->getId()
             << setw(20) << c->getName()
             << setw(12) << type
             << right
             << setw(10) << c->getOrderHistory().size()
             << setw(15) << spent
             << "\n";
    }
}
void MenuSystem::upgradeCustomerToPremium() {
    int id = readInt("Customer ID to upgrade: ");
    double loyalty = readDouble("New loyalty (fraction like 0.10 for 10%): ");

    for (auto*& c : dm.customers()) {
        if (c && c->getId() == id) {
            if (dynamic_cast<PremiumCustomer*>(c)) {
                std::cout << "Already premium.\n";
                return;
            }

            // Create premium replacement, copy history
            auto* newC = new PremiumCustomer(c->getId(), c->getName(), loyalty);
            for (int oid : c->getOrderHistory()) newC->addOrderToHistory(oid);

            delete c;
            c = newC;

            std::cout << "Upgraded to Premium.\n";
            return;
        }
    }

    throw InvalidInputException("Customer ID not found.");
}

// ---------------- Order Menu ----------------

void MenuSystem::orderMenu() {
    while (true) {
        std::cout << "\n--- Order Menu ---\n"
                  << "1. Create Order\n"
                  << "2. List Orders\n"
                  << "3. Add Item to Order\n"
                  << "4. Finalize Order\n"
                  << "0. Back\n";

        int choice = readInt("Select: ");

        switch (choice) {
            case 1: createOrder(); dm.saveAll("data"); break;
            case 2: listOrders(); break;
            case 3: addItemToOrder(); dm.saveAll("data"); break;
            case 4: finalizeOrder(); dm.saveAll("data"); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }
}

void MenuSystem::createOrder() {
    int orderId = readInt("New Order ID: ");
    int customerId = readInt("Customer ID: ");
    std::string date = readLine("Date (YYYY-MM-DD): ");

    Customer* customerPtr = nullptr;
    for (auto* c : dm.customers()) {
        if (c && c->getId() == customerId) {
            customerPtr = c;
            break;
        }
    }

    if (!customerPtr) {
        std::cout << "Customer not found. Create new customer now? (y/n): ";
        char ch;
        std::cin >> ch;
        clearInput();

        if (ch == 'y' || ch == 'Y') {
            std::string name = readLine("Customer Name: ");
            std::cout << "Type (1=Regular, 2=Premium): ";
            int type;
            std::cin >> type;
            clearInput();

            if (type == 2) {
                double loyalty = readDouble("Loyalty (fraction like 0.10 for 10%): ");
                customerPtr = new PremiumCustomer(customerId, name, loyalty);
            } else {
                customerPtr = new RegularCustomer(customerId, name);
            }

            dm.customers().push_back(customerPtr);
            std::cout << "Customer created.\n";
        } else {
            throw InvalidInputException("Order cancelled (customer not found).");
        }
    }

    dm.orders().emplace_back(orderId, customerPtr, date);
    std::cout << "Order created.\n";
}

void MenuSystem::listOrders() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    cout << "\n";
    cout << left
         << setw(10) << "OrderID"
         << setw(12) << "Customer"
         << setw(15) << "Date"
         << right
         << setw(12) << "Total"
         << setw(12) << "Finalized"
         << "\n";

    cout << "---------------------------------------------------------------------\n";

    cout << std::fixed << std::setprecision(2);

    for (const auto& o : dm.orders()) {
        int cid = o.getCustomer() ? o.getCustomer()->getId() : -1;

        cout << left
             << setw(10) << o.getOrderId()
             << setw(12) << cid
             << setw(15) << o.getDate()
             << right
             << setw(12) << o.getTotalAmount()
             << left
             << setw(12) << (o.getIsFinalized() ? "Yes" : "No")
             << "\n";
    }
}
void MenuSystem::addItemToOrder() {
    int orderId = readInt("Order ID: ");
    int productId = readInt("Product ID: ");
    int qty = readInt("Quantity: ");

    Order* orderPtr = nullptr;
    for (auto& o : dm.orders()) {
        if (o.getOrderId() == orderId) { orderPtr = &o; break; }
    }
    if (!orderPtr) throw InvalidInputException("Order not found.");

    Product* productPtr = nullptr;
    for (auto& p : dm.products()) {
        if (p.getId() == productId) { productPtr = &p; break; }
    }
    if (!productPtr) throw InvalidInputException("Product not found.");

    orderPtr->addItem(productPtr, qty);
    orderPtr->calculateTotal(); // show updated subtotal (optional)

    std::cout << "Item added.\n";
}

void MenuSystem::finalizeOrder() {
    int orderId = readInt("Order ID to finalize: ");

    for (auto& o : dm.orders()) {
        if (o.getOrderId() == orderId) {
            o.finalize(dm.finance());
            std::cout << "Order finalized.\n";

            // Print invoice
            o.printInvoice();

            return;
        }
    }

    throw InvalidInputException("Order not found.");
}

// ---------------- Finance Menu ----------------

void MenuSystem::financeMenu() {
    while (true) {
        std::cout << "\n--- Finance Menu ---\n"
                  << "1. Show Summary\n"
                  << "2. List Transactions\n"
                  << "0. Back\n";

        int choice = readInt("Select: ");

        switch (choice) {
            case 1: showFinanceSummary(); break;
            case 2: listTransactions(); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }
}

void MenuSystem::showFinanceSummary() {
    std::cout << "\nTotal Revenue: " << dm.finance().getTotalRevenue() << "\n"
              << "Total Expenses: " << dm.finance().getTotalExpenses() << "\n"
              << "Profit: " << dm.finance().calculateProfit() << "\n";
}

void MenuSystem::listTransactions() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    const auto& tx = dm.finance().getTransactions();

    cout << "\n";
    cout << left
         << setw(12) << "Type"
         << right
         << setw(12) << "Amount"
         << left
         << setw(15) << "Date"
         << setw(25) << "Description"
         << "\n";

    cout << "--------------------------------------------------------------------------\n";

    cout << std::fixed << std::setprecision(2);

    for (const auto& t : tx) {
        cout << left
             << setw(12) << t.type
             << right
             << setw(12) << t.amount
             << left
             << setw(15) << t.date
             << setw(25) << t.description
             << "\n";
    }
}

void MenuSystem::bestSellingProductsReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    // productId -> totalQtySold
    std::unordered_map<int, int> qtySold;

    // 1) Aggregate sales from finalized orders only
    for (const auto& o : dm.orders()) {
        if (!o.getIsFinalized()) continue;

        for (const auto& it : o.getItems()) {
            Product* p = it.first;
            int qty = it.second;
            if (!p) continue;

            qtySold[p->getId()] += qty;
        }
    }

    // 2) Build sortable rows: (productId, totalQtySold)
    std::vector<std::pair<int, int>> rows;
    rows.reserve(qtySold.size());
    for (const auto& kv : qtySold) {
        rows.push_back({kv.first, kv.second});
    }

    // 3) Sort by qty desc
    std::sort(rows.begin(), rows.end(),
              [](const auto& a, const auto& b) {
                  if (a.second != b.second) return a.second > b.second;
                  return a.first < b.first;
              });

    // 4) Print
    cout << "\n=== Best Selling Products (Finalized Orders Only) ===\n";

    if (rows.empty()) {
        cout << "No finalized sales yet.\n";
        return;
    }

    cout << left
         << setw(6)  << "Rank"
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << right
         << setw(12) << "SoldQty"
         << "\n";

    cout << "---------------------------------------------------------\n";

    int rank = 1;
    for (const auto& [pid, sold] : rows) {
        // Find product name from dm.products()
        std::string name = "(unknown)";
        for (const auto& p : dm.products()) {
            if (p.getId() == pid) {
                name = p.getName();
                break;
            }
        }

        cout << left
             << setw(6)  << rank
             << setw(8)  << pid
             << setw(22) << name
             << right
             << setw(12) << sold
             << "\n";

        rank++;
    }
}
void MenuSystem::topCustomersReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    std::unordered_map<int, double> spending;

    for (const auto& o : dm.orders()) {
        if (!o.getIsFinalized()) continue;

        int cid = o.getCustomer()->getId();
        spending[cid] += o.getTotalAmount();
    }

    std::vector<std::pair<int, double>> rows(spending.begin(), spending.end());

    std::sort(rows.begin(), rows.end(),
              [](auto& a, auto& b) {
                  return a.second > b.second;
              });

    cout << "\n=== Top Customers ===\n";

    cout << left
         << setw(6)  << "Rank"
         << setw(8)  << "ID"
         << setw(20) << "Name"
         << right
         << setw(15) << "Total Spent"
         << "\n";

    cout << "------------------------------------------------------\n";
    cout << std::fixed << std::setprecision(2);

    int rank = 1;
    for (auto& [cid, total] : rows) {
        std::string name = "(unknown)";
        for (auto* c : dm.customers()) {
            if (c->getId() == cid) {
                name = c->getName();
                break;
            }
        }

        cout << left
             << setw(6)  << rank++
             << setw(8)  << cid
             << setw(20) << name
             << right
             << setw(15) << total
             << "\n";
    }

    if (rows.empty())
        cout << "No finalized sales yet.\n";
}
void MenuSystem::inventoryValuationReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    double totalValue = 0.0;

    cout << "\n=== Inventory Valuation ===\n";

    cout << left
         << setw(8)  << "ID"
         << setw(20) << "Name"
         << right
         << setw(12) << "Stock"
         << setw(15) << "Value"
         << "\n";

    cout << "------------------------------------------------------\n";
    cout << std::fixed << std::setprecision(2);

    for (const auto& p : dm.products()) {
        double value = p.getQuantity() * p.getCost();
        totalValue += value;

        cout << left
             << setw(8)  << p.getId()
             << setw(20) << p.getName()
             << right
             << setw(12) << p.getQuantity()
             << setw(15) << value
             << "\n";
    }

    cout << "\nTotal Inventory Value: " << totalValue << "\n";
}
void MenuSystem::monthlySalesReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    std::unordered_map<std::string, double> monthly;

    for (const auto& o : dm.orders()) {
        if (!o.getIsFinalized()) continue;

        std::string date = o.getDate();
        std::string month = date.substr(0, 7); // YYYY-MM

        monthly[month] += o.getTotalAmount();
    }

    std::vector<std::pair<std::string, double>> rows(monthly.begin(), monthly.end());

    std::sort(rows.begin(), rows.end());

    cout << "\n=== Monthly Sales Report ===\n";

    cout << left
         << setw(10) << "Month"
         << right
         << setw(15) << "Revenue"
         << "\n";

    cout << "------------------------------\n";
    cout << std::fixed << std::setprecision(2);

    for (auto& [month, revenue] : rows) {
        cout << left
             << setw(10) << month
             << right
             << setw(15) << revenue
             << "\n";
    }

    if (rows.empty())
        cout << "No sales data.\n";
}

void MenuSystem::riskInventoryReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    int highStockThreshold = 50;
    int lowSalesThreshold  = 5;

    // Count total sold quantities
    std::unordered_map<int, int> qtySold;

    for (const auto& o : dm.orders()) {
        if (!o.getIsFinalized()) continue;

        for (const auto& it : o.getItems()) {
            Product* p = it.first;
            int qty = it.second;
            if (!p) continue;

            qtySold[p->getId()] += qty;
        }
    }

    cout << "\n=== Risk Inventory Report ===\n";
    cout << "High Stock > " << highStockThreshold
         << " AND Sold < " << lowSalesThreshold << "\n\n";

    cout << left
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << right
         << setw(12) << "Stock"
         << setw(12) << "Sold"
         << "\n";

    cout << "------------------------------------------------------------\n";

    bool found = false;

    for (const auto& p : dm.products()) {
        int currentStock = p.getQuantity();
        int sold = qtySold[p.getId()];  // default 0 if not sold

        if (currentStock > highStockThreshold &&
            sold < lowSalesThreshold) {

            found = true;

            cout << left
                 << setw(8)  << p.getId()
                 << setw(22) << p.getName()
                 << right
                 << setw(12) << currentStock
                 << setw(12) << sold
                 << "\n";
        }
    }

    if (!found) {
        cout << "No risky inventory detected.\n";
    }
}

void MenuSystem::smartRiskReport() {
    using std::cout;
    using std::left;
    using std::right;
    using std::setw;

    std::unordered_map<int, int> qtySold;

    for (const auto& o : dm.orders()) {
        if (!o.getIsFinalized()) continue;

        for (const auto& it : o.getItems()) {
            qtySold[it.first->getId()] += it.second;
        }
    }

    struct Row {
        int id;
        std::string name;
        int stock;
        int sold;
        double risk;
    };

    std::vector<Row> rows;

    for (const auto& p : dm.products()) {
        int sold = qtySold[p.getId()];
        int stock = p.getQuantity();
        double risk = stock / double(sold + 1);

        rows.push_back({p.getId(), p.getName(), stock, sold, risk});
    }

    std::sort(rows.begin(), rows.end(),
              [](const Row& a, const Row& b) {
                  return a.risk > b.risk;
              });

    cout << "\n=== Smart Risk Inventory Report ===\n";

    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << right
         << setw(10) << "Stock"
         << setw(10) << "Sold"
         << setw(12) << "RiskScore"
         << "\n";

    cout << "------------------------------------------------------------\n";
    cout << std::fixed << std::setprecision(2);

    for (const auto& r : rows) {
        cout << left
             << setw(6)  << r.id
             << setw(20) << r.name
             << right
             << setw(10) << r.stock
             << setw(10) << r.sold
             << setw(12) << r.risk
             << "\n";
    }
}


// ---------------- Reports Menu (placeholder) ----------------

void MenuSystem::reportsMenu() {
    while (true) {
        std::cout << "\n--- Reports Menu ---\n"
                  << "1. Best Selling Products\n"
                  << "2. Risk Inventory\n"
                  << "3. Top Customers\n"
                  << "4. Inventory Valuation\n"
                  << "5. Monthly Sales\n"
                  << "6. Smart Risk Score\n"
                  << "0. Back\n";

        int choice = readInt("Select: ");

        switch (choice) {
            case 1: bestSellingProductsReport(); break;
            case 2: riskInventoryReport(); break;
            case 3: topCustomersReport(); break;
            case 4: inventoryValuationReport(); break;
            case 5: monthlySalesReport(); break;
            case 6: smartRiskReport(); break;
            case 0: return;
            default: std::cout << "Invalid choice.\n"; break;
        }
    }
}