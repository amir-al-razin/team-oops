# API Reference

This reference documents the primary classes and their responsibilities in the Business Management System.

## Core Application Layer

### Application

Purpose: lifecycle orchestration (startup, run loop, shutdown).

Key methods:
- `Application(std::string dataDir = "data")`
- `void initialize()`
- `void run()`
- `void shutdown()`

### MenuSystem

Purpose: command-line UI and user workflows.

Key menus:
- `mainLoop()`
- `inventoryMenu()`
- `customerMenu()`
- `orderMenu()`
- `financeMenu()`
- `reportsMenu()`

Key workflow methods:
- Inventory: `addProduct()`, `restockProduct()`, `removeStockFlow()`, `updateProduct()`, `removeProduct()`, `lowStockAlert()`
- Customer: `addRegularCustomer()`, `addPremiumCustomer()`, `listCustomers()`, `viewCustomerDetails()`, `upgradeCustomerToPremium()`
- Orders: `createOrder()`, `listOrders()`, `viewOrderDetails()`, `addItemToOrder()`, `finalizeOrder()`
- Finance/Reports: `showRevenueSummary()`, `showExpenseSummary()`, `showProfitLossReport()`, `listTransactions()`, `bestSellingProducts()`, `topCustomersReport()`, `inventoryValueReport()`, `monthlySalesReport()`, `smartRiskScoreReport()`

Input helpers:
- `int getIntInput(const std::string& prompt, int min, int max)`
- `double getDoubleInput(const std::string& prompt, double min, double max)`
- `std::string getStringInput(const std::string& prompt)`

## Data Coordination Layer

### DataManager

Purpose: owns aggregate in-memory datasets and delegates persistence to FileManager.

Key methods:
- `std::vector<Product>& products()`
- `std::vector<Customer*>& customers()`
- `std::vector<Order>& orders()`
- `Finance& finance()`
- `void loadAll(const std::string& dir)`
- `void saveAll(const std::string& dir)`

### FileManager

Purpose: read/write text files for all major entities.

Representative methods:
- Product persistence load/save
- Customer persistence load/save
- Order persistence load/save (including load-safe item insertion)
- Finance transaction load/save

## Domain Layer

### Product

Purpose: product catalog item with pricing and stock behavior.

Representative methods:
- Accessors/mutators for id, name, price, stock, reorder level
- `void addStock(int qty)`
- `void removeStock(int qty)`
- `bool isLowStock() const`

### Customer (Base Class)

Purpose: shared customer behavior and polymorphic discount contract.

Representative methods:
- `virtual double calculateDiscount() const = 0`
- order history maintenance (`addOrder`, read history)
- spending accumulation (`addSpending`, total spent)
- `static Customer* upgradeToPremium(const Customer* existing, double loyaltyRate)`

Derived classes:
- `RegularCustomer`
- `PremiumCustomer`

### Order

Purpose: order/cart aggregate linked to customer and order items.

Representative methods:
- `void addItem(Product* product, int qty)`
- `void addLoadedItem(Product* product, int qty)`
- `double calculateTotal() const`
- `void finalize(Finance& finance)`
- `void printInvoice() const`

### Finance

Purpose: track revenue/expense transactions and compute summaries.

Representative methods:
- `void recordRevenue(double amount, const std::string& date, const std::string& note)`
- `void recordExpense(double amount, const std::string& date, const std::string& note)`
- `double getTotalRevenue() const`
- `double getTotalExpense() const`
- `double getNetProfit() const`
- transaction listing APIs

## Utility and Infrastructure

### Repository (Template)

Purpose: generic add/remove/get/list storage abstraction for typed entities.

### Exceptions

Purpose: custom exception hierarchy for domain and input errors.

## Usage Example (High-level)

1. Start app through `Application::initialize()` and `run()`.
2. Use menu actions to mutate products/customers/orders.
3. Finalize orders to update finance ledger.
4. Exit through `shutdown()` to persist data.
