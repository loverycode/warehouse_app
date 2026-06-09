#ifndef ADMIN_H
#define ADMIN_H

#include "core/User.h"
#include "core/Product.h"
#include "core/Stock.h"

class Admin : public User
{
public:
    Admin(int id, const std::string &login, const std::string &password, const std::string &name);

    Product *createProduct(int id, const std::string &sku, const std::string &name, double price);
    bool editProduct(Product *product);
    bool fixStock(Stock *stock, int newQuantity);

    bool canManageProducts() const override { return true; }
    bool canFixStock() const override { return true; }
};

#endif