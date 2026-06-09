#ifndef ORDER_ITEM_H
#define ORDER_ITEM_H

#include "core/Product.h"

class OrderItem
{
private:
    int orderId;
    int productId;
    int quantity;
    double price;

public:
    OrderItem(Product *product, int quantity);
    OrderItem(Product *product, int quantity, double price); // новый конструктор
    OrderItem(int productId, int quantity, double price);    // для загрузки из БД

    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    void setOrderId(int id) { orderId = id; }
};

#endif