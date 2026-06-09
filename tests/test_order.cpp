#include <gtest/gtest.h>
#include "documents/Order.h"
#include "documents/OrderItem.h"
#include "core/Product.h"

class OrderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        order = new Order();
        product1 = new Product(1, "P1", "Товар1", 100.0);
        product2 = new Product(2, "P2", "Товар2", 50.0);
    }
    void TearDown() override
    {
        delete order;
        delete product1;
        delete product2;
    }
    Order *order;
    Product *product1;
    Product *product2;
};

TEST_F(OrderTest, Constructor)
{
    EXPECT_EQ(order->getStatus(), "Создан");
}

TEST_F(OrderTest, AddItemAndTotalPrice)
{
    order->addItem(OrderItem(product1, 2));
    order->addItem(OrderItem(product2, 3));
    EXPECT_DOUBLE_EQ(order->getTotalPrice(), 350.0);
}

TEST_F(OrderTest, RemoveItem)
{
    order->addItem(OrderItem(product1, 2));
    order->addItem(OrderItem(product2, 3));
    order->removeItem(product1->getId());
    EXPECT_DOUBLE_EQ(order->getTotalPrice(), 150.0);
}

TEST_F(OrderTest, CloseOrder)
{
    EXPECT_TRUE(order->close());
    EXPECT_EQ(order->getStatus(), "Закрыт");
}

TEST_F(OrderTest, CancelOrder)
{
    EXPECT_TRUE(order->cancel());
    EXPECT_EQ(order->getStatus(), "Отменён");
}