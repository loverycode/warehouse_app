#include "documents/ReceiptItem.h"

ReceiptItem::ReceiptItem(Product *product, int quantity)
    : productId(product->getId()), quantity(quantity), buyPrice(0.0) {}