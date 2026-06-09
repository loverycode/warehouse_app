#include <gtest/gtest.h>
#include "core/Admin.h"

class AdminTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        admin = new Admin(2, "admin", "admin", "Админ");
    }
    void TearDown() override { delete admin; }
    Admin *admin;
};

TEST_F(AdminTest, Constructor)
{
    EXPECT_EQ(admin->getId(), 2);
}

TEST_F(AdminTest, CreateProduct)
{
    Product *product = admin->createProduct(100, "TEST_SKU", "Test", 999.99);
    EXPECT_NE(product, nullptr);
    delete product;
}

TEST_F(AdminTest, FixStock)
{
    Stock stock(1, 10);
    EXPECT_TRUE(admin->fixStock(&stock, 25));
    EXPECT_EQ(stock.getQuantity(), 25);
}

TEST_F(AdminTest, EditProduct)
{
    Product product(100, "EDIT_SKU", "Old Name", 100.0);
    product.setName("New Name");
    product.setPrice(200.0);
    EXPECT_EQ(product.getName(), "New Name");
    EXPECT_DOUBLE_EQ(product.getPrice(), 200.0);
}

TEST_F(AdminTest, FixStockToZero)
{
    Stock stock(1, 10);
    EXPECT_TRUE(admin->fixStock(&stock, 0));
    EXPECT_EQ(stock.getQuantity(), 0);
}
