#include "RegularCustomer.h"

RegularCustomer::RegularCustomer(int id, const std::string& name) : Customer(id, name) {}

double RegularCustomer::calculateDiscount() const {
    return 0.0;
}