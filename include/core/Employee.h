#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "core/User.h"
#include "documents/Order.h"
#include "documents/Receipt.h"
#include "documents/Shipment.h"
#include "core/Stock.h"
#include <vector>

class Employee : public User
{
public:
    Employee(int id, const std::string &login, const std::string &password, const std::string &name);

    Order *createOrder();
    Receipt *createReceipt();
    Shipment *createShipment();
    std::vector<Stock> viewStock();

    bool canManageProducts() const override { return false; }
    bool canFixStock() const override { return false; }
};

#endif