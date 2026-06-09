#ifndef RECEIPT_ITEM_H
#define RECEIPT_ITEM_H

#include "core/Product.h"

class ReceiptItem
{
    int receiptId;
    int productId;
    int quantity;
    double buyPrice;

public:
    ReceiptItem(Product *product, int quantity);
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getBuyPrice() const { return buyPrice; }
};

#endif