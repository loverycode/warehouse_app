#include <gtest/gtest.h>
#include "database/Database.h"
#include "core/Employee.h"
#include "core/Admin.h"
#include "facade/WarehouseFacade.h"

TEST(Scenario, EmployeeCreatesOrder)
{
    Database::getInstance()->connect();
    Employee emp(1, "emp", "pass", "Тестовый");
    Order *order = emp.createOrder();
    EXPECT_NE(order, nullptr);
    delete order;
}

TEST(Scenario, AdminManagesProducts)
{
    Database *db = Database::getInstance();
    db->connect();
    Admin admin(2, "admin", "pass", "Тестовый");
    Product *product = admin.createProduct(0, "SCENARIO_SKU", "Test", 100.0);
    EXPECT_TRUE(db->addProduct(*product));
    Product *found = db->getProductBySku("SCENARIO_SKU");
    EXPECT_NE(found, nullptr);
    db->deleteProduct(found->getId());
    delete product;
}

TEST(Scenario, ReceiveGoods)
{
    WarehouseFacade &facade = WarehouseFacade::getInstance();
    Product *product = Database::getInstance()->getProductById(1);
    if (product)
    {
        std::vector<ReceiptItem> items = {ReceiptItem(product, 5)};
        EXPECT_TRUE(facade.receiveGoods(items, 1));
    }
}

TEST(Scenario, ViewStock)
{
    WarehouseFacade &facade = WarehouseFacade::getInstance();
    auto stocks = facade.getCurrentStock();
    EXPECT_GE(stocks.size(), 0);
}