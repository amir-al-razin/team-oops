# Implementation Backlog and Bug Tracker

Updated: 2026-03-27

This file tracks what is still pending in the project and the bugs confirmed during code review and runtime checks.

## 1) Confirmed Bugs (Reproducible)

None currently open from the initial bug batch.

## 1.1) Resolved Bugs

### BUG-001: Top Customers report crashed (segmentation fault) - Resolved
- Resolution summary: Added null-customer guards in `topCustomersReport()` and lookup loop.
- Verification summary: report path `5 -> 3` now exits normally (EXIT:0).

### BUG-002: Order loader used stock inflation workaround - Resolved
- Resolution summary: Added `Order::addLoadedItem()` and removed loader stock inflation logic.
- Verification summary: build and report regression tests pass after refactor.

### BUG-003: Declared Customer upgrade API was missing - Resolved
- Resolution summary: Implemented `Customer::upgradeToPremium()` and routed menu upgrade path through it.
- Verification summary: upgrade flow test passes and persistence reflects upgraded type.

## 2) Pending or Partial PRD Tasks

## Phase 4
- Task 4.4 (Partial)
  - Stock-loss workflow exists in menu (`removeStockAsLoss`), but dedicated `Product`-level method signature from PRD is not added.

## Phase 5
- Task 5.2 (Partial)
  - Inventory menu lacks update product and remove product with loss handling.
- Task 5.3 (Partial)
  - Missing single customer detail view with history focus.
- Task 5.4 (Partial)
  - Missing integrated preview/details flow in order creation journey.
- Task 5.5 (Partial)
  - Finance submenu/report coverage is below PRD target.
- Task 5.7 (Partial)
  - Input helpers exist with different names/signatures from PRD utility spec.

## Phase 6
- Task 6.1 (Partial)
  - Application class exists, but explicit initialize/run/shutdown separation is not complete.
- Task 6.2 (Partial)
  - main.cpp exists; top-level exception policy and shutdown orchestration are not fully aligned with PRD.

## Phase 7
- Task 7.1 (Not done)
  - Manual unit test checklist is not documented/executed as a tracked artifact.
- Task 7.2 (Not done)
  - Integration test workflows are not documented/executed as a tracked artifact.
- Task 7.3 (Not done)
  - UAT checklist is not documented/executed as a tracked artifact.

## Phase 8
- Task 8.1 (Partial)
  - Cleanup and consistency pass still pending.
- Task 8.2 (Not done)
  - API reference and full documentation remain incomplete.
- Task 8.3 (Partial)
  - Sample data exists, but README documentation for sample data is pending.
- Task 8.4 (Not done)
  - Final formal verification matrix is pending.

## Phase 9
- Task 9.1 (Not done)
  - Demo script not prepared.
- Task 9.2 (Not done)
  - Submission package checklist not complete.
- Task 9.3 (Not done)
  - Final review checklist not completed.

## 3) Suggested Implementation Order
- Priority 1: Complete remaining Phase 4 gap (4.4 Product-level method/API alignment).
- Priority 2: Complete missing menu/report workflows in Phase 5.
- Priority 3: Finish Phase 6 lifecycle alignment and formal testing phases (7-9).
