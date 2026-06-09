#include "documents/ShipmentItem.h"

ShipmentItem::ShipmentItem(Product *product, int quantity)
    : productId(product->getId()), quantity(quantity), sellPrice(product->getPrice()) {}