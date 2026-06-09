#include "builder/OrderBuilder.h"

OrderBuilder::OrderBuilder() : order(new Order()) {}
OrderBuilder::~OrderBuilder()
{
    if (order)
        delete order;
}

OrderBuilder &OrderBuilder::addItem(Product *product, int quantity)
{
    order->addItem(OrderItem(product, quantity));
    return *this;
}

OrderBuilder &OrderBuilder::setEmployee(Employee *employee)
{
    order->setEmployeeId(employee->getId());
    return *this;
}

Order *OrderBuilder::build()
{
    Order *result = order;
    order = nullptr;
    return result;
}