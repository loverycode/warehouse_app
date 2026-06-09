#include "core/Stock.h"
#include <string>

Stock::Stock(int productId, int quantity, int threshold)
    : productId(productId), quantity(quantity), threshold(threshold) {}

void Stock::updateQuantity(int newQuantity)
{
    quantity = newQuantity;
    if (quantity < threshold)
    {
        notify("Критический остаток товара " + std::to_string(productId));
    }
}