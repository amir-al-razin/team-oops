# Testing Report

Date: 2026-03-27
Scope: Manual unit checks, integration checks, and UAT-style flow validation.

## Environment

- OS: Linux
- Build toolchain: `g++` + `make`
- Build mode: `-std=c++17 -Wall -Wextra`

## 1) Clean Build Verification

Command:

```bash
make clean && make all
```

Result:
- PASS
- Full rebuild completed and executable generated.

## 2) Manual Unit-Level Checks

These are focused, feature-level checks executed through the CLI.

### Product and Inventory checks

Command path:
- `Inventory -> List Products`

Result:
- PASS
- Product table prints correctly with ID, price, stock, reorder level.

### Customer checks

Command path:
- `Customer -> List Customers`

Result:
- PASS
- Customer type, order count, and total spent values print correctly.

### Order checks

Command path:
- `Order -> List Orders`

Result:
- PASS
- Order table shows id, customer id, date, total, and finalized status.

### Invalid input checks

Command:

```bash
printf 'x\n9\n0\n' | ./BusinessManagementSystem
```

Observed output includes:
- `Invalid number. Try again.`
- `Input must be between 0 and 5.`

Result:
- PASS

Additional edge command:

```bash
printf -- '-1\nabc\n0\n' | ./BusinessManagementSystem
```

Observed output includes:
- `Input must be between 0 and 5.`
- `Invalid number. Try again.`

Result:
- PASS

## 3) Integration Testing

### Workflow A: End-to-end transaction

Goal:
- Create order
- Add item
- Finalize order
- Verify finance transaction

Scripted flow result excerpts:
- `Order created.`
- `Order finalized.`
- Finance transaction contains `Order #99001`

Result:
- PASS

### Workflow B: Persistence across sessions

Goal:
- Create new customer
- Exit application
- Restart and verify customer exists

Result excerpt:
- Customer row includes `9901  Test Persist User`

Result:
- PASS

## 4) UAT-Style Navigation & Reports

### Full menu navigation

Command path:
- Main -> Inventory -> Customer -> Order -> Finance -> Reports -> Exit

Result:
- PASS
- All menus open/return correctly.

### Finance reports

Command path:
- Finance: Revenue, Expense, Profit/Loss, Transaction History

Result:
- PASS

### System reports

Command path:
- Reports: Best Sellers, Risk Inventory, Top Customers, Inventory Valuation, Monthly Sales, Smart Risk Score

Result:
- PASS
- Includes regression confirmation for previously crashing Top Customers path.

## 5) Data Safety During Testing

- Mutable tests were performed with a pre-test backup (`/tmp/team-oops-data-backup-final`).
- This ensures repeatability and safe rollback capability if needed.

## Overall Result

- Unit-level checks: PASS
- Integration checks: PASS
- UAT flow checks: PASS
- Clean build verification: PASS

No open reproducible crash from the tested scenarios.
