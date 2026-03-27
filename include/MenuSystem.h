#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include "DataManager.h"

class MenuSystem {
private:
    DataManager& dm;

    // Helpers
    static void clearInput();
    static int readInt(const char* prompt);
    static double readDouble(const char* prompt);
    static std::string readLine(const char* prompt);

    // Menus
    void inventoryMenu();
    void customerMenu();
    void orderMenu();
    void financeMenu();
    void reportsMenu();

    // Inventory actions
    void addProduct();
    void restockProduct();
    void removeStock(Product& product, int qty, const std::string& reason);
    void removeStockAsLoss();
    void updateProduct();
    void removeProduct();
    void listProducts();
    void lowStockAlert();
    void inventoryValuationReport();

    // Customer actions
    void addRegularCustomer();
    void addPremiumCustomer();
    void listCustomers();
    void viewCustomerDetails();
    void upgradeCustomerToPremium();
    void topCustomersReport();

    // Order actions
    void createOrder();
    void listOrders();
    void viewOrderDetails();
    void addItemToOrder();
    void finalizeOrder();

    // Finance actions
    void showRevenueSummary();
    void showExpenseSummary();
    void showProfitLossReport();
    void showFinanceSummary();
    void listTransactions();
    void bestSellingProductsReport();
    void riskInventoryReport();
    void monthlySalesReport();
    void smartRiskReport();

public:
    explicit MenuSystem(DataManager& dm);
    void mainLoop();
};

#endif