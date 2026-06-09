#include "documents/OrderItem.h"

OrderItem::OrderItem(Product *product, int quantity)
    : productId(product->getId()), quantity(quantity), price(product->getPrice()) {}

OrderItem::OrderItem(Product *product, int quantity, double price)
    : productId(product->getId()), quantity(quantity), price(price) {}

OrderItem::OrderItem(int productId, int quantity, double price)
    : productId(productId), quantity(quantity), price(price) {}