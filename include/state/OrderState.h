#ifndef ORDER_STATE_H
#define ORDER_STATE_H

class Order;
class Product;

class OrderState
{
public:
    virtual ~OrderState() = default;
    virtual void addItem(Order *order, Product *product, int qty) = 0;
    virtual void close(Order *order) = 0;
    virtual void cancel(Order *order) = 0;
};

#endif