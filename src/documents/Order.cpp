#include "documents/Order.h"
#include "state/CreatedState.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

Order::Order() : id(0), status("Создан"), state(new CreatedState())
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    date = oss.str();
}

Order::Order(int id, const std::string &number, int employeeId)
    : id(id), number(number), employeeId(employeeId), status("Создан"), state(new CreatedState())
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    date = oss.str();
}

// Конструктор копирования
Order::Order(const Order &other)
    : id(other.id), number(other.number), date(other.date),
      status(other.status), employeeId(other.employeeId), items(other.items)
{
    state = new CreatedState();
}

// Оператор присваивания
Order &Order::operator=(const Order &other)
{
    if (this != &other)
    {
        id = other.id;
        number = other.number;
        date = other.date;
        status = other.status;
        employeeId = other.employeeId;
        items = other.items;

        delete state;
        state = new CreatedState();
    }
    return *this;
}

Order::~Order()
{
    delete state;
}

void Order::addItem(const OrderItem &item)
{
    items.push_back(item);
}

void Order::doAddItem(Product *product, int qty)
{
    addItem(OrderItem(product, qty));
}

void Order::removeItem(int productId)
{
    items.erase(std::remove_if(items.begin(), items.end(),
                               [productId](const OrderItem &item)
                               { return item.getProductId() == productId; }),
                items.end());
}

bool Order::close()
{
    if (status == "Создан")
    {
        status = "Закрыт";
        return true;
    }
    return false;
}

bool Order::cancel()
{
    if (status == "Создан")
    {
        status = "Отменён";
        return true;
    }
    return false;
}

double Order::getTotalPrice() const
{
    double total = 0;
    for (const auto &item : items)
    {
        total += item.getPrice() * item.getQuantity();
    }
    return total;
}

void Order::setState(OrderState *newState)
{
    if (state)
        delete state;
    state = newState;
}