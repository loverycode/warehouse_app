#include "state/CreatedState.h"
#include "documents/Order.h"
#include "state/ClosedState.h"
#include "state/CancelledState.h"

void CreatedState::addItem(Order *order, Product *product, int qty)
{
    order->doAddItem(product, qty);
}

void CreatedState::close(Order *order)
{
    order->setState(new ClosedState());
    order->setStatus("Закрыт");
}

void CreatedState::cancel(Order *order)
{
    order->setState(new CancelledState());
    order->setStatus("Отменён");
}