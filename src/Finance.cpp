#include "Finance.h"
#include <iostream>
#include <iomanip>

using namespace std;

Finance::Finance() : totalRevenue(0.0), totalExpenses(0.0) {}

double Finance::getTotalRevenue() const {
    return totalRevenue;
}

double Finance::getTotalExpenses() const {
    return totalExpenses;
}

const vector<Finance::Transaction>& Finance::getTransactions() const {
    return transactions;
}

void Finance::recordExpense(double amount, const string& desc, const string& date) {
    if (amount < 0) {
        throw invalid_argument("Expense amount cannot be negative.");
    }
    totalExpenses += amount;
    transactions.push_back({"Expense", amount, date.empty() ? "N/A" : date, desc});
}

void Finance::recordRevenue(double amount, const string& desc, const string& date) {
    if (amount < 0) {
        throw invalid_argument("Revenue amount cannot be negative.");
    }
    totalRevenue += amount;
    transactions.push_back({"Revenue", amount, date.empty() ? "N/A" : date, desc});
}

double Finance::calculateProfit() const {
    return totalRevenue - totalExpenses;
}

void Finance::generateReport() const {
    cout << fixed << setprecision(2);
    cout << "=== Financial Report ===" << endl;
    cout << "Total Revenue: $" << totalRevenue << endl;
    cout << "Total Expenses: $" << totalExpenses << endl;
    cout << "Profit/Loss: $" << calculateProfit() << endl;
    cout << "\nTransaction History:" << endl;
    if (transactions.empty()) {
        cout << "No transactions recorded." << endl;
    } else {
        for (const auto& t : transactions) {
            cout << t.type << ": $" << t.amount << " on " << t.date << " - " << t.description << endl;
        }
    }
    cout << "========================" << endl;
}