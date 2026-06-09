#ifndef ORDER_BUILDER_H
#define ORDER_BUILDER_H

#include "documents/Order.h"
#include "core/Product.h"
#include "core/Employee.h"

class OrderBuilder
{
    Order *order;

public:
    OrderBuilder();
    ~OrderBuilder();
    OrderBuilder &addItem(Product *product, int quantity);
    OrderBuilder &setEmployee(Employee *employee);
    Order *build();
};

#endif