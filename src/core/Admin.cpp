#include "core/Admin.h"

Admin::Admin(int id, const std::string &login, const std::string &password, const std::string &name)
    : User(id, login, password, name, Role::Admin) {}

Product *Admin::createProduct(int id, const std::string &sku, const std::string &name, double price)
{
    return new Product(id, sku, name, price);
}

bool Admin::editProduct(Product *product)
{
    // TODO: реализовать
    return true;
}

bool Admin::fixStock(Stock *stock, int newQuantity)
{
    stock->updateQuantity(newQuantity);
    return true;
}