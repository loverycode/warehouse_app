#include "documents/Receipt.h"
#include "database/Database.h"
#include <ctime>
#include <sstream>
#include <iomanip>

Receipt::Receipt() : id(0)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    date = oss.str();
}

Receipt::Receipt(int id, const std::string &number, const std::string &date, const std::string &supplier, int employeeId)
    : id(id), number(number), date(date), supplier(supplier), employeeId(employeeId) {}

void Receipt::addItem(const ReceiptItem &item)
{
    items.push_back(item);
}

void Receipt::save()
{
    if (number.empty())
    {
        number = "RCP-" + std::to_string(std::time(nullptr));
    }
    Database::getInstance()->saveReceipt(*this);

    for (const auto &item : items)
    {
        Stock *stock = Database::getInstance()->getStock(item.getProductId());
        if (stock)
        {
            stock->updateQuantity(stock->getQuantity() + item.getQuantity());
            Database::getInstance()->updateStock(item.getProductId(), stock->getQuantity());
        }
    }
}