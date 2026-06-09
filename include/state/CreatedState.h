#ifndef CREATED_STATE_H
#define CREATED_STATE_H

#include "state/OrderState.h"

class CreatedState : public OrderState
{
public:
    void addItem(Order *order, Product *product, int qty) override;
    void close(Order *order) override;
    void cancel(Order *order) override;
};

#endif