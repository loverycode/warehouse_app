#include <gtest/gtest.h>
#include "core/Employee.h"

class EmployeeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        employee = new Employee(1, "emp", "pass", "Сотрудник");
    }
    void TearDown() override { delete employee; }
    Employee *employee;
};

TEST_F(EmployeeTest, Constructor)
{
    EXPECT_EQ(employee->getId(), 1);
}

TEST_F(EmployeeTest, CreateOrder)
{
    Order *order = employee->createOrder();
    EXPECT_NE(order, nullptr);
    delete order;
}

TEST_F(EmployeeTest, CreateReceipt)
{
    Receipt *receipt = employee->createReceipt();
    EXPECT_NE(receipt, nullptr);
    delete receipt;
}

TEST_F(EmployeeTest, CreateShipment)
{
    Shipment *shipment = employee->createShipment();
    EXPECT_NE(shipment, nullptr);
    delete shipment;
}

TEST_F(EmployeeTest, ViewStock)
{
    auto stocks = employee->viewStock();
    EXPECT_GE(stocks.size(), 0);
}