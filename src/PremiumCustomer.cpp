#include "PremiumCustomer.h"

PremiumCustomer::PremiumCustomer(int id, const std::string& name, double loyaltyPercentage)
    : Customer(id, name), loyaltyPercentage(loyaltyPercentage) {}

double PremiumCustomer::calculateDiscount() const {
    return loyaltyPercentage / 100.0;  // Assuming loyaltyPercentage is in percent, return as fraction
}

double PremiumCustomer::getLoyaltyPercentage() const {
    return loyaltyPercentage;
}