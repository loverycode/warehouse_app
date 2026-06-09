#ifndef STOCK_H
#define STOCK_H

#include "observer/Subject.h"

class Stock : public Subject
{
    int productId;
    int quantity;
    int threshold;

public:
    Stock(int productId, int quantity, int threshold = 5);
    void updateQuantity(int newQuantity);
    int getQuantity() const { return quantity; }
    int getProductId() const { return productId; }
};

#endif