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

## Phase 5
- Task 5.7 (Done)
  - PRD-style input utility APIs are implemented and wired into menu selection paths.

## Phase 6
- Task 6.1 (Done)
  - Application lifecycle is split into explicit `initialize()`, `run()`, and `shutdown()` methods.
- Task 6.2 (Done)
  - Main entry calls initialize/run/shutdown, handles top-level exceptions, and includes Ctrl+C shutdown handling.

## Phase 7
- Task 7.1 (Done)
  - Manual unit-level testing documented and executed in `docs/TESTING_REPORT.md`.
- Task 7.2 (Done)
  - Integration workflows documented and executed in `docs/TESTING_REPORT.md`.
- Task 7.3 (Done)
  - UAT-style navigation/input/report checks documented in `docs/TESTING_REPORT.md`.

## Phase 8
- Task 8.1 (Done)
  - Cleanup/consistency pass completed.
- Task 8.2 (Done)
  - `README.md` and `docs/API_REFERENCE.md` completed.
- Task 8.3 (Done)
  - Sample data documented in `README.md`.
- Task 8.4 (Partial)
  - Final verification matrix completed in `docs/FINAL_VERIFICATION_MATRIX.md`; fresh-environment/Codespaces run remains.

## Phase 9
- Task 9.1 (Done)
  - Demo script prepared in `docs/DEMO_SCRIPT.md`.
- Task 9.2 (Done)
  - Submission package checklist completed in `docs/SUBMISSION_CHECKLIST.md`.
- Task 9.3 (Done)
  - Final review checklist completed in `docs/SUBMISSION_CHECKLIST.md` and PRD status.

## 3) Suggested Implementation Order
- Priority 1: Run one clean verification in a fresh environment (GitHub Codespaces or equivalent).
