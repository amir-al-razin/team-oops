# Business Management System - Implementation Plan

**Project:** CLI-based Business Management System  
**Language:** C++  
**Environment:** GitHub Codespaces (Linux/MinGW g++)  
**Curriculum:** CSE 4301 - Object-Oriented Programming

---

## Overview

This implementation plan provides a structured, task-based approach to building the Business Management System from the ground up. Each phase builds upon the previous one, ensuring a systematic development process that aligns with OOP curriculum topics.

---

## Phase 0: Project Setup & Environment Configuration

### Task 0.1: Initialize Project Structure
- [x] Use repository root as project directory
- [x] Create directories:
  - `src/` - Source files (.cpp)
  - `include/` - Header files (.h)
  - `data/` - Data storage files (.txt)
  - `docs/` - Documentation
- [x] Initialize Git repository
- [x] Create `.gitignore` for compiled files (`*.o`, `*.exe`, `*.out`)

### Task 0.2: Setup Build Environment
- [x] Create `Makefile` or `CMakeLists.txt` for compilation
- [x] Test compiler availability (`g++ --version`)
- [x] Create a simple "Hello World" test program
- [x] Verify compilation and execution

### Task 0.3: Documentation Setup
- [x] Copy PRD to `docs/PRD.md`
- [x] Create `docs/API_REFERENCE.md` (to be filled incrementally)
- [x] Create `README.md` with project description and build instructions

---

## Phase 1: Foundation - Core Data Models

### Task 1.1: Define the Product Class
**Curriculum: Week 3 - Objects & Classes, Encapsulation**

- [x] Create `include/Product.h`
  - Define private attributes: `id`, `name`, `price`, `cost`, `quantity`
  - Declare public methods: constructors, getters, setters
  - Declare methods: `updateStock()`, `calculateProfit()`
- [x] Create `src/Product.cpp`
  - Implement constructor with validation
  - Implement getter/setter methods
  - Implement `updateStock(int qty)` with bounds checking
  - Implement `calculateProfit()` as `(price - cost) * quantity`
- [x] Add `toString()` or display method for debugging

### Task 1.2: Define the Customer Base Class
**Curriculum: Week 6 - Inheritance**

- [x] Create `include/Customer.h`
  - Define abstract base class `Customer`
  - Private attributes: `id`, `name`, `vector<int> orderHistory`
  - Declare pure virtual method: `virtual double calculateDiscount() = 0`
  - Declare methods: `addOrderToHistory()`, `getOrderHistory()`
- [x] Create `src/Customer.cpp`
  - Implement constructor
  - Implement `addOrderToHistory(int orderId)`
  - Implement non-virtual methods

### Task 1.3: Implement Customer Derived Classes
**Curriculum: Weeks 8-9 - Virtual Functions & Polymorphism**

- [ ] Create `include/RegularCustomer.h`
  - Inherit from `Customer`
  - Override `calculateDiscount()` to return `0.0`
- [ ] Create `src/RegularCustomer.cpp`
  - Implement constructor (call base constructor)
  - Implement `calculateDiscount()` override
- [ ] Create `include/PremiumCustomer.h`
  - Inherit from `Customer`
  - Add attribute: `loyaltyPercentage` (default 5-10%)
  - Override `calculateDiscount()` to return loyalty percentage
- [ ] Create `src/PremiumCustomer.cpp`
  - Implement constructor with loyalty parameter
  - Implement `calculateDiscount()` override

### Task 1.4: Define the Order Class
**Curriculum: Week 3 - Objects & Classes**

- [ ] Create `include/Order.h`
  - Attributes: `orderId`, `Customer* customer`, `vector<pair<Product*, int>> items`, `date`, `totalAmount`, `isFinalized`
  - Methods: `addItem()`, `removeItem()`, `calculateTotal()`, `finalize()`
- [ ] Create `src/Order.cpp`
  - Implement constructor
  - Implement `addItem(Product* product, int qty)` with stock validation
  - Implement `calculateTotal()` with discount application
  - Implement `finalize()` to lock the order and trigger updates

### Task 1.5: Define the Finance Class
**Curriculum: Week 3 - Objects & Classes**

- [ ] Create `include/Finance.h`
  - Attributes: `totalRevenue`, `totalExpenses`, `vector<Transaction> transactions`
  - Inner struct/class: `Transaction` (type, amount, date, description)
  - Methods: `recordExpense()`, `recordRevenue()`, `calculateProfit()`, `generateReport()`
- [ ] Create `src/Finance.cpp`
  - Implement `recordExpense(double amount, string desc)`
  - Implement `recordRevenue(double amount, string desc)`
  - Implement `calculateProfit()` as `totalRevenue - totalExpenses`
  - Implement `generateReport()` to display summary

---

## Phase 2: Advanced Features - Operators & Templates

### Task 2.1: Implement Operator Overloading for Product
**Curriculum: Week 5 - Operator Overloading**

- [ ] In `Product.h`, declare comparison operators:
  - `bool operator>(const Product& other) const`
  - `bool operator<(const Product& other) const`
  - `bool operator==(const Product& other) const`
- [ ] In `Product.cpp`, implement operators:
  - Compare by price for `>` and `<`
  - Compare by ID for `==`
- [ ] Add `operator<<` for `ostream` (optional for easy printing)

### Task 2.2: Create Generic Repository Template
**Curriculum: Week 11 - Templates & Generics**

- [ ] Create `include/Repository.h` (header-only template)
  - Template class: `template<typename T> class Repository`
  - Private attribute: `vector<T> items`
  - Methods:
    - `void add(const T& item)`
    - `bool remove(int id)`
    - `T* getById(int id)`
    - `vector<T> getAll()`
    - `int size()`
- [ ] Implement all methods inline in the header
- [ ] Add error handling for invalid IDs

### Task 2.3: Implement Exception Handling
**Curriculum: Exception Handling**

- [ ] Create `include/Exceptions.h`
  - Define custom exceptions:
    - `class InsufficientStockException : public std::exception`
    - `class InvalidInputException : public std::exception`
    - `class FileOperationException : public std::exception`
  - Override `what()` method for each
- [ ] Integrate exception throwing in:
  - `Product::updateStock()` - throw if stock goes negative
  - `Order::addItem()` - throw if insufficient stock
  - File operations - throw on file open/write failures

---

## Phase 3: Data Persistence - File I/O

### Task 3.1: Design Data File Format
**Curriculum: Week 10 - Streams & File I/O**

- [ ] Define CSV-like format for `data/products.txt`:
  ```
  ID,Name,Price,Cost,Quantity
  ```
- [ ] Define format for `data/customers.txt`:
  ```
  ID,Name,Type,LoyaltyPercentage,OrderIDs (comma-separated)
  ```
- [ ] Define format for `data/orders.txt`:
  ```
  OrderID,CustomerID,Date,TotalAmount,Finalized,Items (product_id:qty;...)
  ```
- [ ] Define format for `data/finance.txt`:
  ```
  TotalRevenue,TotalExpenses
  TransactionType,Amount,Date,Description
  ```

### Task 3.2: Implement FileManager Class
**Curriculum: Week 10 - Streams & File I/O**

- [ ] Create `include/FileManager.h`
  - Static methods for each entity type:
    - `static vector<Product> loadProducts(string filepath)`
    - `static void saveProducts(const vector<Product>&, string filepath)`
    - `static vector<Customer*> loadCustomers(string filepath)`
    - `static void saveCustomers(const vector<Customer*>&, string filepath)`
    - Similar for Orders and Finance
- [ ] Create `src/FileManager.cpp`
  - Implement `loadProducts()`:
    - Open `ifstream`
    - Read line by line
    - Parse CSV
    - Create Product objects
    - Handle exceptions
  - Implement `saveProducts()`:
    - Open `ofstream`
    - Write header
    - Write each product as CSV line
    - Close file
  - Repeat for other entity types
  - **Important:** Handle polymorphism for Customer (store type field, reconstruct RegularCustomer or PremiumCustomer)

### Task 3.3: Implement Auto-Save Mechanism
- [ ] Create `DataManager` wrapper class:
  - Holds repositories for all entities
  - Method: `saveAll()` - calls FileManager save methods
  - Method: `loadAll()` - calls FileManager load methods
- [ ] Integrate auto-save:
  - Call `saveAll()` after every critical operation (add/remove/update)
  - Call `saveAll()` on application exit

---

## Phase 4: Business Logic & Automation

### Task 4.1: Implement Inventory Alerts
- [ ] In `Product` class, add method:
  - `bool isLowStock(int threshold = 10)`
- [ ] Create utility function in main or InventoryManager:
  - `void checkLowStock(Repository<Product>& repo)`
  - Iterate through products, display warnings

### Task 4.2: Implement Order Finalization Logic
- [ ] In `Order::finalize()`:
  - Calculate total with discount using `customer->calculateDiscount()`
  - Iterate through items and call `product->updateStock(-qty)`
  - Call `Finance::recordRevenue(totalAmount)`
  - Add order ID to customer's history
  - Set `isFinalized = true`
- [ ] Add validation: cannot finalize twice

### Task 4.3: Implement Stock Addition as Expense
- [ ] When adding new product or restocking:
  - Calculate expense: `cost * quantity`
  - Call `Finance::recordExpense(expense, "Stock purchase")`

### Task 4.4: Implement Stock Removal (Loss Handling)
- [ ] Create method `void removeStock(Product& product, int qty, string reason)`
  - Reduce quantity without revenue
  - If reason == "damage" or "expiry", record as loss in finance

### Task 4.5: Implement Customer Upgrade Logic
- [ ] Create function `void upgradeToPreium(Customer*& customer, double loyaltyPercent)`:
  - Create new PremiumCustomer with same ID, name, history
  - Delete old RegularCustomer
  - Update pointer
  - Update repository

---

## Phase 5: User Interface - Menu System

### Task 5.1: Design Main Menu Structure
- [ ] Create `include/MenuSystem.h`
  - Class `MenuSystem` with methods for each menu
  - Method: `void displayMainMenu()`
  - Method: `void handleMainMenuChoice(int choice)`
- [ ] Create hierarchy:
  ```
  Main Menu
    1. Inventory Management
    2. Customer Management
    3. Order Processing
    4. Financial Reports
    5. System Reports
    6. Exit
  ```

### Task 5.2: Implement Inventory Management Menu
- [ ] Create submenu:
  ```
  Inventory Menu
    1. Add New Product
    2. View All Products
    3. Update Product
    4. Remove Product
    5. Check Low Stock
    6. Back to Main Menu
  ```
- [ ] Implement each option:
  - Option 1: Prompt for product details, validate, add to repository, record expense
  - Option 2: Display all products in formatted table
  - Option 3: Get product by ID, prompt for new values, update
  - Option 4: Remove product, handle as loss
  - Option 5: Call low stock check function

### Task 5.3: Implement Customer Management Menu
- [ ] Create submenu:
  ```
  Customer Menu
    1. Register New Customer
    2. View All Customers
    3. View Customer Details
    4. Upgrade to Premium
    5. Back to Main Menu
  ```
- [ ] Implement each option:
  - Option 1: Prompt for name, type, create appropriate customer object
  - Option 2: Display all customers with type
  - Option 3: Show customer info + order history
  - Option 4: Upgrade logic

### Task 5.4: Implement Order Processing Menu
- [ ] Create submenu:
  ```
  Order Menu
    1. Create New Order
    2. View All Orders
    3. View Order Details
    4. Back to Main Menu
  ```
- [ ] Implement "Create New Order" workflow:
  - Select customer by ID
  - Display available products
  - Add items to cart (loop)
  - Show preview with discount
  - Confirm and finalize
  - Handle exceptions (insufficient stock)

### Task 5.5: Implement Financial Reports Menu
- [ ] Create submenu:
  ```
  Finance Menu
    1. View Revenue Summary
    2. View Expense Summary
    3. Profit/Loss Report
    4. Transaction History
    5. Back to Main Menu
  ```
- [ ] Implement each report using Finance class methods

### Task 5.6: Implement System Reports Menu
- [ ] Create submenu:
  ```
  Reports Menu
    1. Best Selling Products
    2. Top Customers
    3. Inventory Valuation
    4. Risk Inventory (High Stock, Low Sales)
    5. Back to Main Menu
  ```
- [ ] Implement analytics:
  - Best Sellers: Count order occurrences, sort products
  - Top Customers: Sum order totals per customer, sort
  - Inventory Value: Sum `price * quantity` for all products
  - Risk Inventory: Products with high stock but low sales frequency

### Task 5.7: Implement Input Validation & Error Handling
- [ ] Create utility functions:
  - `int getIntInput(string prompt, int min, int max)`
  - `double getDoubleInput(string prompt, double min, double max)`
  - `string getStringInput(string prompt)`
- [ ] Wrap all inputs in try-catch blocks
- [ ] Clear input buffer on invalid input
- [ ] Display user-friendly error messages

---

## Phase 6: Integration & Main Application

### Task 6.1: Create Application Controller
**Curriculum: Weeks 12-13 - Multi-file Programming**

- [ ] Create `include/Application.h`
  - Class `Application` (Singleton pattern optional)
  - Attributes: repositories, finance, menuSystem
  - Methods: `initialize()`, `run()`, `shutdown()`
- [ ] Create `src/Application.cpp`
  - `initialize()`: Load all data from files
  - `run()`: Main loop calling menu system
  - `shutdown()`: Save all data, cleanup

### Task 6.2: Implement Main Entry Point
- [ ] Create `src/main.cpp`
  - Create Application instance
  - Call `initialize()`
  - Call `run()`
  - Handle top-level exceptions
  - Call `shutdown()` on exit or Ctrl+C

### Task 6.3: Integrate All Components
- [ ] Link all classes together in Application
- [ ] Ensure repositories are passed correctly
- [ ] Verify polymorphism works (Customer pointers)
- [ ] Test file save/load cycle

---

## Phase 7: Testing & Validation

### Task 7.1: Unit Testing (Manual)
- [ ] Test Product class:
  - Create products, verify getters/setters
  - Test stock update with negative values (should throw)
  - Test operator overloading
- [ ] Test Customer classes:
  - Create Regular and Premium customers
  - Verify discount calculation
  - Test order history
- [ ] Test Repository:
  - Add, remove, get operations
  - Test with different types (Product, Customer)
- [ ] Test FileManager:
  - Save data, delete memory, load data, verify integrity

### Task 7.2: Integration Testing
- [ ] Test complete workflows:
  - Add product → Create customer → Create order → Finalize → Check finance
  - Verify stock reduction
  - Verify discount application
  - Verify revenue recording
- [ ] Test edge cases:
  - Order with 0 items
  - Order exceeding stock
  - File corruption handling

### Task 7.3: User Acceptance Testing
- [ ] Test all menu navigation paths
- [ ] Test invalid inputs (letters for numbers, negative values)
- [ ] Test file persistence across sessions
- [ ] Verify all reports display correctly

---

## Phase 8: Polish & Documentation

### Task 8.1: Code Cleanup
- [ ] Remove debug/test code
- [ ] Ensure consistent naming conventions
- [ ] Add comments to complex logic
- [ ] Format code (indentation, spacing)
- [ ] Check for memory leaks (Customer pointers)

### Task 8.2: Complete Documentation
- [ ] Fill `API_REFERENCE.md`:
  - Document all classes with their methods
  - Include usage examples
- [ ] Update `README.md`:
  - Installation instructions
  - Compilation commands
  - Usage guide
  - Features list
- [ ] Add inline code documentation (Doxygen-style optional)

### Task 8.3: Create Sample Data
- [ ] Populate `data/` folder with sample files:
  - 10-15 sample products
  - 5-7 sample customers (mix of Regular/Premium)
  - 3-5 sample orders
  - Initial finance state
- [ ] Document sample data in README

### Task 8.4: Final Testing
- [ ] Clean build test (delete all .o files, rebuild)
- [ ] Test on fresh environment (GitHub Codespaces)
- [ ] Verify all curriculum topics are demonstrated
- [ ] Cross-reference with PRD requirements (FR-01 through FR-14)

---

## Phase 9: Submission Preparation

### Task 9.1: Create Demonstration Script
- [ ] Write step-by-step demo walkthrough:
  - Add product
  - Create customer
  - Process order
  - Generate reports
- [ ] Practice demonstration

### Task 9.2: Prepare Submission Package
- [ ] Create project archive with:
  - All source/header files
  - Makefile/build instructions
  - Documentation
  - Sample data
  - Demo script
- [ ] Verify archive extracts and builds correctly

### Task 9.3: Final Review
- [ ] Review PRD checklist:
  - All functional requirements implemented
  - All non-functional requirements met
  - All curriculum topics covered
- [ ] Test on a clean system one final time

---

## Curriculum Topic Checklist

| Topic | Implementation | Files Involved |
|-------|---------------|----------------|
| **Week 3: Objects & Classes** | Product, Customer, Order, Finance classes | All entity headers/source files |
| **Week 5: Operator Overloading** | Product comparison operators | `Product.h/.cpp` |
| **Week 6: Inheritance** | Customer → RegularCustomer, PremiumCustomer | `Customer.h`, derived classes |
| **Week 8-9: Virtual Functions** | `calculateDiscount()` polymorphism | Customer hierarchy |
| **Week 10: File I/O** | Save/load all data | `FileManager.h/.cpp` |
| **Week 11: Templates** | Generic Repository | `Repository.h` |
| **Week 12-13: Multi-file** | Header/implementation separation | All `.h` and `.cpp` files |
| **Exception Handling** | Custom exceptions, try-catch | `Exceptions.h`, usage throughout |

---

## Build Commands Reference

```bash
# Single file compilation (for testing)
g++ -std=c++17 -I./include -o test src/Product.cpp src/main.cpp

# Full project build with Makefile
make clean
make all

# Run application
./BusinessManagementSystem
```

---

## Success Criteria

- ✅ All 14 functional requirements (FR-01 to FR-14) implemented
- ✅ All 4 non-functional requirements met
- ✅ All 8 curriculum topics demonstrated
- ✅ Data persists across sessions
- ✅ No crashes on invalid input
- ✅ Clean compilation with no warnings
- ✅ Code follows header/implementation structure
- ✅ Documentation complete and accurate

---

## Notes

- **Memory Management:** Be careful with Customer pointers in Repository. Consider using smart pointers (`unique_ptr`, `shared_ptr`) if allowed by curriculum.
- **Date Handling:** Use simple string format (YYYY-MM-DD) or C++ `<ctime>` for dates.
- **Makefile:** Create incrementally as you add files to avoid compilation issues.
- **Git Commits:** Commit after each major task completion for version control.

---

**End of Implementation Plan**

This plan is designed to be followed sequentially, with each phase building on the previous one. Feel free to adjust task granularity based on your working pace, but maintain the phase order to ensure dependencies are met.