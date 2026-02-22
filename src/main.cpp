#include <iostream>
#include "Product.h"
#include "RegularCustomer.h"
#include "PremiumCustomer.h"
#include "Order.h"
#include "Finance.h"

int main() {
    try {
        // Create product
        Product laptop(1, "Laptop", 1000, 800, 10);

        // Create customer
        PremiumCustomer customer(101, "Alice", 0.10); // 10% discount

        // Create finance
        Finance finance;

        // Create order
        Order order(1, &customer, "2026-02-23");

        // Add item
        order.addItem(&laptop, 2);

        // Before finalize
        std::cout << "Stock before: " << laptop.getQuantity() << "\n";

        // Finalize
        order.finalize(finance);

        // After finalize
        std::cout << "Stock after: " << laptop.getQuantity() << "\n";
        std::cout << "Order total: " << order.getTotalAmount() << "\n";
        std::cout << "Finance revenue: " << finance.getTotalRevenue() << "\n";

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}