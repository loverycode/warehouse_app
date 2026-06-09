#include <gtest/gtest.h>
#include "database/Database.h"

class DatabaseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        db = Database::getInstance();
        db->connect();
    }
    void TearDown() override { db->disconnect(); }
    Database *db;
};

TEST_F(DatabaseTest, SingletonWorks)
{
    Database *db2 = Database::getInstance();
    EXPECT_EQ(db, db2);
}

TEST_F(DatabaseTest, AddAndDeleteProduct)
{
    Product newProduct(0, "TEST_SKU", "Test", 100.0);
    EXPECT_TRUE(db->addProduct(newProduct));
    Product *found = db->getProductBySku("TEST_SKU");
    EXPECT_NE(found, nullptr);
    EXPECT_TRUE(db->deleteProduct(found->getId()));
}

TEST_F(DatabaseTest, GetProductById)
{
    Product *product = db->getProductById(1);
    EXPECT_NE(product, nullptr);
}

TEST_F(DatabaseTest, UpdateStock)
{
    Stock *stock = db->getStock(1);
    if (stock)
    {
        int oldQty = stock->getQuantity();
        EXPECT_TRUE(db->updateStock(1, oldQty + 10));
    }
}

TEST_F(DatabaseTest, GetProductBySku)
{
    Product newProduct(0, "SKU_FIND_TEST", "Find Test", 123.45);
    db->addProduct(newProduct);

    Product *found = db->getProductBySku("SKU_FIND_TEST");
    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getName(), "Find Test");

    db->deleteProduct(found->getId());
}