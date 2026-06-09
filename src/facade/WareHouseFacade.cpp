#include "facade/WarehouseFacade.h"

WarehouseFacade *WarehouseFacade::instance = nullptr;

WarehouseFacade &WarehouseFacade::getInstance()
{
    if (!instance)
        instance = new WarehouseFacade();
    return *instance;
}

bool WarehouseFacade::receiveGoods(const std::vector<ReceiptItem> &items, int employeeId)
{
    // TODO: реализовать
    return true;
}

bool WarehouseFacade::shipOrder(int orderId)
{
    // TODO: реализовать
    return true;
}

std::vector<Stock> WarehouseFacade::getCurrentStock()
{
    return {}; // заглушка
}