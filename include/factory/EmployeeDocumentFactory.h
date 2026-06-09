#ifndef EMPLOYEE_DOCUMENT_FACTORY_H
#define EMPLOYEE_DOCUMENT_FACTORY_H

#include "factory/DocumentFactory.h"

class EmployeeDocumentFactory : public DocumentFactory
{
public:
    Order *createOrder() override;
    Receipt *createReceipt() override;
    Shipment *createShipment() override;
};

#endif