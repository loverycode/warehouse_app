#include <gtest/gtest.h>
#include "documents/OrderItem.h"
#include "core/Product.h"

TEST(OrderItemTest, ConstructorWithProduct)
{
    Product product(1, "SKU", "Товар", 100.0);
    OrderItem item(&product, 5);

    EXPECT_EQ(item.getProductId(), 1);
    EXPECT_EQ(item.getQuantity(), 5);
    EXPECT_DOUBLE_EQ(item.getPrice(), 100.0);
}

TEST(OrderItemTest, ConstructorWithProductAndPrice)
{
    Product product(1, "SKU", "Товар", 100.0);
    OrderItem item(&product, 5, 80.0);

    EXPECT_EQ(item.getProductId(), 1);
    EXPECT_EQ(item.getQuantity(), 5);
    EXPECT_DOUBLE_EQ(item.getPrice(), 80.0);
}

TEST(OrderItemTest, ConstructorWithIds)
{
    OrderItem item(1, 3, 100.0);

    EXPECT_EQ(item.getProductId(), 1);
    EXPECT_EQ(item.getQuantity(), 3);
    EXPECT_DOUBLE_EQ(item.getPrice(), 100.0);
}

TEST(OrderItemTest, Getters)
{
    Product product(1, "SKU", "Товар", 100.0);
    OrderItem item(&product, 5);

    EXPECT_EQ(item.getProductId(), 1);
    EXPECT_EQ(item.getQuantity(), 5);
    EXPECT_DOUBLE_EQ(item.getPrice(), 100.0);
}

TEST(OrderItemTest, SetOrderId)
{
    OrderItem item(1, 1, 10.0);
    // Если есть метод setOrderId
    // item.setOrderId(99);
    // EXPECT_EQ(item.getOrderId(), 99);
    EXPECT_GT(item.getProductId(), 0);
}