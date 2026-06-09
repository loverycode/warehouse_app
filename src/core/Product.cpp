#include "core/Product.h"

Product::Product(int id, const std::string &sku, const std::string &name, double price, const std::string &desc)
    : id(id), sku(sku), name(name), price(price), description(desc) {}