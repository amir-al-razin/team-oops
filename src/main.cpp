#include <iostream>
#include <iomanip>

#include "Product.h"
#include "Customer.h"
#include "RegularCustomer.h"
#include "PremiumCustomer.h"
#include "Order.h"
#include "Finance.h"
#include "Exceptions.h"

static void printOrder(const Order& order) {
    std::cout << "\n=== Order Summary ===\n";
    std::cout << "Order ID: " << order.getOrderId() << "\n";
    std::cout << "Customer: " << (order.getCustomer() ? order.getCustomer()->getName() : "N/A") << "\n";
    std::cout << "Date: " << order.getDate() << "\n";
    std::cout << "Finalized: " << (order.getIsFinalized() ? "Yes" : "No") << "\n";
    std::cout << "Items:\n";

    for (const auto& [p, qty] : order.getItems()) {
        std::cout << "  - " << p->getName()
                  << " (id=" << p->getId() << ")"
                  << " x " << qty
                  << " @ $" << std::fixed << std::setprecision(2) << p->getPrice()
                  << "\n";
    }

    std::cout << "Total Amount: $" << std::fixed << std::setprecision(2)
              << order.getTotalAmount() << "\n";
    std::cout << "=====================\n";
}

int main() {
    try {
        std::cout << "===== Business Management System Compile/Run Test =====\n";

        // Products (id, name, price, cost, quantity)
        Product laptop(1, "Laptop", 1200.0, 900.0, 5);
        Product mouse(2, "Mouse", 25.0, 10.0, 20);

        std::cout << "\nInitial Products:\n";
        std::cout << laptop << "\n";
        std::cout << mouse << "\n";

        // Customers
        RegularCustomer regular(101, "Alice");
        PremiumCustomer premium(102, "Bob", 0.10); // 10% discount (as fraction)

        std::cout << "\nDiscounts:\n";
        std::cout << regular.getName() << ": " << (regular.calculateDiscount() * 100) << "%\n";
        std::cout << premium.getName() << ": " << (premium.calculateDiscount() * 100) << "%\n";

        // Finance
        Finance finance;

        // Order 1 (orderId, customer*, date)
        Order o1(1, &regular, "2026-02-23");
        o1.addItem(&laptop, 1);
        o1.addItem(&mouse, 2);
        o1.calculateTotal();
        o1.finalize();

        laptop.updateStock(-1);
        mouse.updateStock(-2);
        finance.recordRevenue(o1.getTotalAmount(), "Order #1", o1.getDate());

        printOrder(o1);

        // Order 2
        Order o2(2, &premium, "2026-02-23");
        o2.addItem(&laptop, 1);
        o2.calculateTotal();
        o2.finalize();

        laptop.updateStock(-1);
        finance.recordRevenue(o2.getTotalAmount(), "Order #2", o2.getDate());

        printOrder(o2);

        std::cout << "\nProducts After Orders:\n";
        std::cout << laptop << "\n";
        std::cout << mouse << "\n";

        std::cout << "\nFinance Report:\n";
        finance.generateReport();

        std::cout << "\nIf you see this, build + runtime worked.\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\n Exception: " << e.what() << "\n";
        return 1;
    }
}