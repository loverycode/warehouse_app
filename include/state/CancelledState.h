#ifndef CANCELLED_STATE_H
#define CANCELLED_STATE_H

#include "state/OrderState.h"

class CancelledState : public OrderState
{
public:
    void addItem(Order *order, Product *product, int qty) override;
    void close(Order *order) override;
    void cancel(Order *order) override;
};

#endif