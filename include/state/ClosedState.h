#ifndef CLOSED_STATE_H
#define CLOSED_STATE_H

#include "state/OrderState.h"

class ClosedState : public OrderState
{
public:
    void addItem(Order *order, Product *product, int qty) override;
    void close(Order *order) override;
    void cancel(Order *order) override;
};

#endif