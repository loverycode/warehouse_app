#include <gtest/gtest.h>
#include "documents/Receipt.h"
#include "documents/ReceiptItem.h"
#include "core/Product.h"

TEST(ReceiptTest, Constructor)
{
    Receipt receipt;
    EXPECT_FALSE(receipt.getDate().empty());
}

TEST(ReceiptTest, AddItem)
{
    Receipt receipt;
    Product product(1, "SKU", "Товар", 100.0);
    receipt.addItem(ReceiptItem(&product, 10));
    EXPECT_EQ(receipt.getItems().size(), 1);
}

TEST(ReceiptTest, Save)
{
    Receipt receipt;
    receipt.setSupplier("Тест");
    receipt.setEmployeeId(1);
    Product product(1, "SKU", "Товар", 100.0);
    receipt.addItem(ReceiptItem(&product, 10));
    EXPECT_NO_THROW(receipt.save());
}

TEST(ReceiptTest, SetAndGetId)
{
    Receipt receipt;
    receipt.setId(100);
    EXPECT_EQ(receipt.getId(), 100);
}

TEST(ReceiptTest, SetAndGetNumber)
{
    Receipt receipt;
    receipt.setNumber("RCP-001");
    EXPECT_EQ(receipt.getNumber(), "RCP-001");
}