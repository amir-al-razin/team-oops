#ifndef PREMIUMCUSTOMER_H
#define PREMIUMCUSTOMER_H

#include "Customer.h"

class PremiumCustomer : public Customer {
private:
    double loyaltyPercentage;

public:
    PremiumCustomer(int id, const std::string& name, double loyaltyPercentage = 5.0);
    virtual double calculateDiscount() const override;
    double getLoyaltyPercentage() const;
};

#endif