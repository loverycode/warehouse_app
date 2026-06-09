#include <gtest/gtest.h>
#include "core/Stock.h"

class StockTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stock = new Stock(1, 10, 5);
    }
    void TearDown() override { delete stock; }
    Stock *stock;
};

TEST_F(StockTest, ConstructorAndGetters)
{
    EXPECT_EQ(stock->getProductId(), 1);
    EXPECT_EQ(stock->getQuantity(), 10);
}

TEST_F(StockTest, UpdateQuantityIncrease)
{
    stock->updateQuantity(15);
    EXPECT_EQ(stock->getQuantity(), 15);
}

TEST_F(StockTest, UpdateQuantityDecrease)
{
    stock->updateQuantity(3);
    EXPECT_EQ(stock->getQuantity(), 3);
}

TEST_F(StockTest, MultipleUpdates)
{
    stock->updateQuantity(5);
    EXPECT_EQ(stock->getQuantity(), 5);
    stock->updateQuantity(8);
    EXPECT_EQ(stock->getQuantity(), 8);
    stock->updateQuantity(2);
    EXPECT_EQ(stock->getQuantity(), 2);
}

TEST_F(StockTest, ThresholdCheck)
{
    Stock high(1, 10, 5);
    Stock low(1, 3, 5);
    Stock equal(1, 5, 5);

    EXPECT_FALSE(high.getQuantity() < 5);
    EXPECT_TRUE(low.getQuantity() < 5);
    EXPECT_FALSE(equal.getQuantity() < 5);
}