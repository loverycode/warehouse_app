#include "factory/EmployeeDocumentFactory.h"

Order *EmployeeDocumentFactory::createOrder() { return new Order(); }
Receipt *EmployeeDocumentFactory::createReceipt() { return new Receipt(); }
Shipment *EmployeeDocumentFactory::createShipment() { return new Shipment(); }