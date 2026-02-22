#include <iostream>
#include "DataManager.h"

int main() {
    try {
        DataManager dm;

        // 1) Load everything
        dm.loadAll("data");
        std::cout << "Loaded:\n";
        std::cout << "  Products: " << dm.products().size() << "\n";
        std::cout << "  Customers: " << dm.customers().size() << "\n";
        std::cout << "  Orders: " << dm.orders().size() << "\n";
        std::cout << "  Profit: " << dm.finance().calculateProfit() << "\n";

        // 2) Modify something
        dm.products().push_back(Product(999, "TestItem", 50, 30, 10));
        dm.finance().recordExpense(100, "Test expense", "2026-02-23");

        // 3) Save everything
        dm.saveAll("data");
        std::cout << "Saved all to data/*.txt\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    }
}