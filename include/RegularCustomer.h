#ifndef REGULARCUSTOMER_H
#define REGULARCUSTOMER_H

#include "Customer.h"

class RegularCustomer : public Customer {
public:
    RegularCustomer(int id, const std::string& name);
    virtual double calculateDiscount() const override;
};

#endif