# Business Management System

CLI-based Business Management System implemented in C++ for CSE 4301 (Object-Oriented Programming).

## Features

- Inventory management:
	- Add product
	- Restock product
	- Remove stock with loss reason and finance expense entry
	- Update product details
	- Remove product (with stock-loss protection)
	- Low stock alerts and risk views
- Customer management:
	- Add regular/premium customers
	- List and inspect customer details
	- Upgrade regular customer to premium
- Order management:
	- Create order and add items through cart-style flow
	- Finalize order and generate invoice
	- View order details and list all orders
- Finance:
	- Revenue summary
	- Expense summary
	- Profit/loss report
	- Full transaction history
- Reports:
	- Best-selling products
	- Risk inventory
	- Top customers
	- Inventory valuation
	- Monthly sales
	- Smart risk score

## Requirements

- Linux/macOS terminal or Windows terminal
- C++17 compiler (`g++` recommended)
- `make`

## Build

```bash
make clean
make all
```

## Run

```bash
./BusinessManagementSystem
```

## Project Structure

- `src/`: class implementations and main entry point
- `include/`: headers/interfaces
- `data/`: persisted sample data (products, customers, orders, finance)
- `docs/`: PRD, API reference, test reports, backlog, and delivery docs

## Sample Data

The project ships with sample data inside `data/`:

- `products.txt`: 50 sample products with stock and reorder levels
- `customers.txt`: regular and premium customer mix
- `orders.txt`: finalized and draft orders (including edge cases)
- `finance.txt`: revenue and expense ledger transactions

This data allows immediate demo/testing without manual bootstrapping.

## Persistence Behavior

- Data is loaded on startup.
- Data is saved on normal shutdown.
- If initial load fails, the app enters a save-protection mode to avoid overwriting existing files.

## Documentation

- PRD and implementation checklist: `docs/PRD.md`
- API reference: `docs/API_REFERENCE.md`
- Testing evidence: `docs/TESTING_REPORT.md`
- Final verification matrix: `docs/FINAL_VERIFICATION_MATRIX.md`
- Demo walkthrough: `docs/DEMO_SCRIPT.md`
- Submission checklist: `docs/SUBMISSION_CHECKLIST.md`

- ### System Architecture
```mermaid
classDiagram
    class Application {
        -DataManager dm
        -MenuSystem menu
        -string dataDir
        -bool safeToSave
        +initialize()
        +run()
        +shutdown()
    }

    class DataManager {
        -vector~Product~ m_products
        -vector~Customer*~ m_customers
        -vector~Order~ m_orders
        -Finance m_finance
        +loadAll(dataDir)
        +saveAll(dataDir)
        +products()
        +customers()
        +orders()
        +finance()
        -clearCustomers()
    }

    class MenuSystem {
        -DataManager dm
        +mainLoop()
        +inventoryMenu()
        +customerMenu()
        +orderMenu()
        +financeMenu()
        +reportsMenu()
    }

    class Product {
        -int id
        -string name
        -double price
        -double cost
        -int quantity
        +updateStock(qty)
        +calculateProfit()
        +isLowStock(threshold)
    }

    class Customer {
        <<abstract>>
        -int id
        -string name
        -vector~int~ orderHistory
        +calculateDiscount()* double
        +addOrderToHistory(orderId)
        +upgradeToPremium(customer, loyaltyPercent)
    }

    class RegularCustomer {
        +calculateDiscount() double
    }

    class PremiumCustomer {
        -double loyaltyPercentage
        +calculateDiscount() double
    }

    class Order {
        -int orderId
        -Customer* customer
        -string date
        -double totalAmount
        -bool isFinalized
        -vector~pair~ items
        +addItem(product, qty)
        +calculateTotal() double
        +finalize(finance)
        +printInvoice()
    }

    class Finance {
        -double totalRevenue
        -double totalExpenses
        -vector~Transaction~ transactions
        +recordExpense(amount, desc, date)
        +recordRevenue(amount, desc, date)
        +calculateProfit() double
        +generateReport()
    }

    Application *-- DataManager : owns
    Application *-- MenuSystem : owns
    MenuSystem --> DataManager : uses
    DataManager *-- Product : manages
    DataManager *-- Customer : manages
    DataManager *-- Order : manages
    DataManager *-- Finance : manages
    
    Customer <|-- RegularCustomer : inherits
    Customer <|-- PremiumCustomer : inherits
    
    Order o-- Customer : references
    Order o-- Product : contains
