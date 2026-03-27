# Demo Script

Estimated duration: 8-10 minutes
Date: 2026-03-27

## Demo Goal

Show a complete business workflow in the CLI system:
1. Inventory action
2. Customer action
3. Order processing
4. Finance and reports
5. Persistence check

## Pre-Demo Setup

1. Build application:

```bash
make clean && make all
```

2. Run application:

```bash
./BusinessManagementSystem
```

## Live Walkthrough

### Step 1: Inventory snapshot

- Navigate: `1 -> 6`
- Explain: Product catalog, stock levels, reorder thresholds.
- Return: `0`

### Step 2: Customer operations

- Navigate: `2 -> 3`
- Explain: Regular vs Premium customers and order history counts.
- Optional detail view: `2 -> 4`, enter a customer ID.
- Return: `0`

### Step 3: Create and finalize an order

- Navigate: `3 -> 1`
- Enter new order ID, customer ID, date.
- Add at least one item.
- Finalize now = `y`.
- Show invoice output.
- Return to menu.

### Step 4: Financial impact

- Navigate: `4 -> 1` (Revenue)
- Navigate: `4 -> 3` (Profit/Loss)
- Navigate: `4 -> 4` (Transaction history)
- Explain the order-to-finance linkage.

### Step 5: Business reports

- Navigate: `5 -> 1`, `5 -> 3`, `5 -> 6`
- Explain trend/risk visibility for managerial decisions.

### Step 6: Exit and persistence

- Exit with `0`.
- Mention that data saves on shutdown.
- Re-open app and list data to show persistence.

## Key Talking Points

- OOP design: clear class responsibilities.
- Inheritance + polymorphism: customer discount model.
- File I/O: persistent records across sessions.
- Error handling: robust validation and safe failure behavior.
- Reporting: operational and decision-support outputs.
