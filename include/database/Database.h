#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <memory>
#include <libpq-fe.h>
#include "core/Product.h"
#include "core/Stock.h"
#include "core/User.h"
#include "documents/Order.h"
#include "documents/OrderItem.h"
#include "documents/Receipt.h"
#include "documents/ReceiptItem.h"
#include "documents/Shipment.h"
#include "documents/ShipmentItem.h"

class Database
{
private:
    static Database *instance;
    PGconn *connection;
    std::string connectionString;

    Database();
    ~Database();

    bool executeQuery(const std::string &sql);
    PGresult *executeSelect(const std::string &sql);

public:
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;
    static Database *getInstance();

    bool connect();
    void disconnect();
    bool isConnected() const { return connection != nullptr && PQstatus(connection) == CONNECTION_OK; }

    // Пользователи
    User *getUserByLogin(const std::string &login);
    bool saveUser(const std::string &login, const std::string &password, const std::string &name, const std::string &role);

    // Товары
    std::vector<Product> getAllProducts();
    Product *getProductById(int id);
    Product *getProductBySku(const std::string &sku);
    bool addProduct(const Product &product);
    bool updateProduct(const Product &product);
    bool deleteProduct(int id);

    // Остатки
    Stock *getStock(int productId);
    bool updateStock(int productId, int quantity);
    std::vector<Stock> getAllStocks();

    // Заказы
    std::vector<Order> getAllOrders();
    Order *getOrderById(int id);
    int saveOrder(const Order &order);
    bool updateOrderStatus(int id, const std::string &status);

    // Документы
    int saveReceipt(const Receipt &receipt);
    int saveShipment(const Shipment &shipment);
    std::vector<Receipt> getAllReceipts();
    std::vector<Shipment> getAllShipments();

    // Журнал
    bool logAction(int userId, const std::string &action, const std::string &details);
};

#endif