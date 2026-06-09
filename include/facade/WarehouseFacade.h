#ifndef WAREHOUSE_FACADE_H
#define WAREHOUSE_FACADE_H

#include <vector>
#include "core/Stock.h"
#include "documents/Receipt.h"

class WarehouseFacade
{
private:
    static WarehouseFacade *instance;
    WarehouseFacade() = default;

public:
    static WarehouseFacade &getInstance();
    bool receiveGoods(const std::vector<ReceiptItem> &items, int employeeId);
    bool shipOrder(int orderId);
    std::vector<Stock> getCurrentStock();
};

#endif