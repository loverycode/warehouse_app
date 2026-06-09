#ifndef SHIPMENT_ITEM_H
#define SHIPMENT_ITEM_H

#include "core/Product.h"

class ShipmentItem
{
    int shipmentId;
    int productId;
    int quantity;
    double sellPrice;

public:
    ShipmentItem(Product *product, int quantity);
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getSellPrice() const { return sellPrice; }
};

#endif