#ifndef DOCUMENT_FACTORY_H
#define DOCUMENT_FACTORY_H

#include "documents/Order.h"
#include "documents/Receipt.h"
#include "documents/Shipment.h"

class DocumentFactory
{
public:
    virtual ~DocumentFactory() = default;
    virtual Order *createOrder() = 0;
    virtual Receipt *createReceipt() = 0;
    virtual Shipment *createShipment() = 0;
};

#endif