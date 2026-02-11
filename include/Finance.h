#ifndef FINANCE_H
#define FINANCE_H

#include <string>
#include <vector>

class Finance {
public:
    struct Transaction {
        std::string type;
        double amount;
        std::string date;
        std::string description;
    };

    Finance();

    double getTotalRevenue() const;
    double getTotalExpenses() const;
    const std::vector<Transaction>& getTransactions() const;

    void recordExpense(double amount, const std::string& desc, const std::string& date = "");
    void recordRevenue(double amount, const std::string& desc, const std::string& date = "");
    double calculateProfit() const;
    void generateReport() const;

private:
    double totalRevenue;
    double totalExpenses;
    std::vector<Transaction> transactions;
};

#endif