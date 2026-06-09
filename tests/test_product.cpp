#include <gtest/gtest.h>
#include "core/Product.h"

class ProductTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        product = new Product(1, "SKU001", "Ноутбук", 50000.0);
    }
    void TearDown() override { delete product; }
    Product *product;
};

TEST_F(ProductTest, Constructor)
{
    EXPECT_EQ(product->getId(), 1);
    EXPECT_EQ(product->getName(), "Ноутбук");
}

TEST_F(ProductTest, SetPrice)
{
    product->setPrice(45000.0);
    EXPECT_DOUBLE_EQ(product->getPrice(), 45000.0);
}

TEST_F(ProductTest, SetName)
{
    product->setName("Игровой ноутбук");
    EXPECT_EQ(product->getName(), "Игровой ноутбук");
}

TEST_F(ProductTest, SetDescription)
{
    product->setDescription("Игровой ноутбук с RTX 4060");
    EXPECT_EQ(product->getDescription(), "Игровой ноутбук с RTX 4060");
}

TEST_F(ProductTest, GetDescription)
{
    Product productWithDesc(2, "SKU002", "Телефон", 30000.0, "Смартфон");
    EXPECT_EQ(productWithDesc.getDescription(), "Смартфон");
}