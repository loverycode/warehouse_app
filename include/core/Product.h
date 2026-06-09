#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product
{
private:
    int id;
    std::string sku;
    std::string name;
    double price;
    std::string description;

public:
    Product(int id, const std::string &sku, const std::string &name, double price, const std::string &desc = "");

    int getId() const { return id; }
    std::string getSku() const { return sku; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    std::string getDescription() const { return description; }

    void setPrice(double newPrice) { price = newPrice; }
    void setName(const std::string &newName) { name = newName; }
    void setDescription(const std::string &newDesc) { description = newDesc; }
};

#endif