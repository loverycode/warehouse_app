#include "core/Employee.h"
#include "factory/EmployeeDocumentFactory.h"
#include "facade/WarehouseFacade.h"

Employee::Employee(int id, const std::string &login, const std::string &password, const std::string &name)
    : User(id, login, password, name, Role::Employee) {}

Order *Employee::createOrder()
{
    return EmployeeDocumentFactory().createOrder();
}

Receipt *Employee::createReceipt()
{
    return EmployeeDocumentFactory().createReceipt();
}

Shipment *Employee::createShipment()
{
    return EmployeeDocumentFactory().createShipment();
}

std::vector<Stock> Employee::viewStock()
{
    return WarehouseFacade::getInstance().getCurrentStock();
}