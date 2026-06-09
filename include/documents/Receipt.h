#ifndef RECEIPT_H
#define RECEIPT_H

#include <string>
#include <vector>
#include "documents/ReceiptItem.h"

class Receipt
{
private:
    int id;
    std::string number;
    std::string date;
    std::string supplier;
    int employeeId;
    std::vector<ReceiptItem> items;

public:
    Receipt();
    Receipt(int id, const std::string &number, const std::string &date, const std::string &supplier, int employeeId);

    void addItem(const ReceiptItem &item);
    void save();

    // Getters and setters
    int getId() const { return id; }
    void setId(int newId) { id = newId; }
    std::string getNumber() const { return number; }
    void setNumber(const std::string &num) { number = num; }
    std::string getDate() const { return date; }
    void setDate(const std::string &d) { date = d; }
    std::string getSupplier() const { return supplier; }
    void setSupplier(const std::string &s) { supplier = s; }
    int getEmployeeId() const { return employeeId; }
    void setEmployeeId(int empId) { employeeId = empId; }
    const std::vector<ReceiptItem> &getItems() const { return items; }
};

#endif