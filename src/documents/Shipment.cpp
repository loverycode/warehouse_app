#include "documents/Shipment.h"
#include "database/Database.h"
#include <ctime>
#include <sstream>
#include <iomanip>

Shipment::Shipment() : id(0)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    date = oss.str();
}

Shipment::Shipment(int id, const std::string &number, const std::string &date, int orderId, int employeeId)
    : id(id), number(number), date(date), orderId(orderId), employeeId(employeeId) {}

void Shipment::addItem(const ShipmentItem &item)
{
    items.push_back(item);
}

void Shipment::save()
{
    Database::getInstance()->saveShipment(*this);
    for (const auto &item : items)
    {
        Stock *stock = Database::getInstance()->getStock(item.getProductId());
        if (stock)
        {
            stock->updateQuantity(stock->getQuantity() - item.getQuantity());
            Database::getInstance()->updateStock(item.getProductId(), stock->getQuantity());
        }
    }

    // Закрываем заказ
    Database::getInstance()->updateOrderStatus(orderId, "Закрыт");
}