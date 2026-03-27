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
