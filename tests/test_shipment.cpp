#include <gtest/gtest.h>
#include "documents/Shipment.h"
#include "documents/ShipmentItem.h"
#include "core/Product.h"

TEST(ShipmentTest, Constructor)
{
    Shipment shipment;
    EXPECT_FALSE(shipment.getDate().empty());
}

TEST(ShipmentTest, AddItem)
{
    Shipment shipment;
    Product product(1, "SKU", "Товар", 100.0);
    shipment.addItem(ShipmentItem(&product, 5));
    EXPECT_EQ(shipment.getItems().size(), 1);
}

TEST(ShipmentTest, Save)
{
    Shipment shipment;
    shipment.setOrderId(1);
    shipment.setEmployeeId(1);
    Product product(1, "SKU", "Товар", 100.0);
    shipment.addItem(ShipmentItem(&product, 5));
    EXPECT_NO_THROW(shipment.save());
}

TEST(ShipmentTest, SetAndGetId)
{
    Shipment shipment;
    shipment.setId(100);
    EXPECT_EQ(shipment.getId(), 100);
}

TEST(ShipmentTest, SetAndGetNumber)
{
    Shipment shipment;
    shipment.setNumber("SHP-001");
    EXPECT_EQ(shipment.getNumber(), "SHP-001");
}