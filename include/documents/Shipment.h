#ifndef SHIPMENT_H
#define SHIPMENT_H

#include <string>
#include <vector>
#include "documents/ShipmentItem.h"

class Shipment
{
private:
    int id;
    std::string number;
    std::string date;
    int orderId;
    int employeeId;
    std::vector<ShipmentItem> items;

public:
    Shipment();
    Shipment(int id, const std::string &number, const std::string &date, int orderId, int employeeId);

    void addItem(const ShipmentItem &item);
    void save();

    // Getters and setters
    int getId() const { return id; }
    void setId(int newId) { id = newId; }
    std::string getNumber() const { return number; }
    void setNumber(const std::string &num) { number = num; }
    std::string getDate() const { return date; }
    void setDate(const std::string &d) { date = d; }
    int getOrderId() const { return orderId; }
    void setOrderId(int oId) { orderId = oId; }
    int getEmployeeId() const { return employeeId; }
    void setEmployeeId(int empId) { employeeId = empId; }
    const std::vector<ShipmentItem> &getItems() const { return items; }
};

#endif