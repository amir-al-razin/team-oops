#include "Finance.h"
#include <iostream>
#include <iomanip>

Finance::Finance() : totalRevenue(0.0), totalExpenses(0.0) {}

double Finance::getTotalRevenue() const {
    return totalRevenue;
}

double Finance::getTotalExpenses() const {
    return totalExpenses;
}

const std::vector<Finance::Transaction>& Finance::getTransactions() const {
    return transactions;
}

void Finance::recordExpense(double amount, const std::string& desc, const std::string& date) {
    if (amount < 0) {
        throw std::invalid_argument("Expense amount cannot be negative.");
    }
    totalExpenses += amount;
    transactions.push_back({"Expense", amount, date.empty() ? "N/A" : date, desc});
}

void Finance::recordRevenue(double amount, const std::string& desc, const std::string& date) {
    if (amount < 0) {
        throw std::invalid_argument("Revenue amount cannot be negative.");
    }
    totalRevenue += amount;
    transactions.push_back({"Revenue", amount, date.empty() ? "N/A" : date, desc});
}

double Finance::calculateProfit() const {
    return totalRevenue - totalExpenses;
}

void Finance::generateReport() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "=== Financial Report ===" << std::endl;
    std::cout << "Total Revenue: $" << totalRevenue << std::endl;
    std::cout << "Total Expenses: $" << totalExpenses << std::endl;
    std::cout << "Profit/Loss: $" << calculateProfit() << std::endl;
    std::cout << "\nTransaction History:" << std::endl;
    if (transactions.empty()) {
        std::cout << "No transactions recorded." << std::endl;
    } else {
        for (const auto& t : transactions) {
            std::cout << t.type << ": $" << t.amount << " on " << t.date << " - " << t.description << std::endl;
        }
    }
    std::cout << "========================" << std::endl;
}