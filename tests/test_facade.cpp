#include <gtest/gtest.h>
#include "facade/WarehouseFacade.h"

TEST(FacadeTest, GetInstance)
{
    WarehouseFacade &f1 = WarehouseFacade::getInstance();
    WarehouseFacade &f2 = WarehouseFacade::getInstance();
    EXPECT_EQ(&f1, &f2);
}

TEST(FacadeTest, GetCurrentStock)
{
    WarehouseFacade &facade = WarehouseFacade::getInstance();
    auto stocks = facade.getCurrentStock();
    EXPECT_GE(stocks.size(), 0);
}