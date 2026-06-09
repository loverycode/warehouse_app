#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "documents/OrderItem.h"

class OrderState;

class Order
{
private:
    int id;
    std::string number;
    std::string date;
    std::string status;
    int employeeId;
    std::vector<OrderItem> items;
    OrderState *state;

public:
    Order();
    Order(int id, const std::string &number, int employeeId);
    Order(const Order &other); // ← конструктор копирования
    ~Order();

    Order &operator=(const Order &other); // ← оператор присваивания

    void addItem(const OrderItem &item);
    void doAddItem(Product *product, int qty);
    void removeItem(int productId);
    bool close();
    bool cancel();
    double getTotalPrice() const;

    void setState(OrderState *newState);
    OrderState *getState() const { return state; }

    // Getters and setters
    int getId() const { return id; }
    void setId(int newId) { id = newId; }
    std::string getNumber() const { return number; }
    void setNumber(const std::string &num) { number = num; }
    std::string getDate() const { return date; }
    void setDate(const std::string &d) { date = d; }
    std::string getStatus() const { return status; }
    void setStatus(const std::string &s) { status = s; }
    int getEmployeeId() const { return employeeId; }
    void setEmployeeId(int empId) { employeeId = empId; }
    const std::vector<OrderItem> &getItems() const { return items; }
};

#endif