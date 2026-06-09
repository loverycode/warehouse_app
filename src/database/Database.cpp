#include "database/Database.h"
#include "core/Employee.h"
#include <QMessageBox>
#include "core/Admin.h"
#include <iostream>
#include <sstream>
#include <QDebug>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <cstdlib>

std::string formatDouble(double value)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string s = ss.str();
    std::replace(s.begin(), s.end(), ',', '.');
    return s;
}

Database *Database::instance = nullptr;

std::string getEnvOrDefault(const std::string &var, const std::string &defaultValue)
{
    const char *val = std::getenv(var.c_str());
    return val ? std::string(val) : defaultValue;
}

Database::Database() : connection(nullptr)
{
    std::string host = getEnvOrDefault("PGHOST", "localhost");
    std::string port = getEnvOrDefault("PGPORT", "5432");
    std::string dbname = getEnvOrDefault("PGDATABASE", "warehouse_db");
    std::string user = getEnvOrDefault("PGUSER", "arinazyranova");
    std::string password = getEnvOrDefault("PGPASSWORD", "");

    connectionString = "host=" + host + " port=" + port +
                       " dbname=" + dbname +
                       " user=" + user +
                       " password=" + password;
}

Database::~Database()
{
    disconnect();
}

Database *Database::getInstance()
{
    if (!instance)
        instance = new Database();
    return instance;
}

bool Database::connect()
{
    if (connection)
        return true;

    connection = PQconnectdb(connectionString.c_str());

    if (PQstatus(connection) != CONNECTION_OK)
    {
        std::string error = PQerrorMessage(connection);
        std::cerr << "Ошибка подключения: " << error << std::endl;

        QString msg = "Не удалось подключиться к БД!\n\n";
        msg += "Строка подключения: " + QString::fromStdString(connectionString) + "\n\n";
        msg += "Ошибка: " + QString::fromStdString(error);

        QMessageBox::critical(nullptr, "Ошибка БД", msg);
        return false;
    }

    std::cout << "Подключено к PostgreSQL" << std::endl;
    return true;
}

void Database::disconnect()
{
    if (connection)
    {
        PQfinish(connection);
        connection = nullptr;
    }
}

bool Database::executeQuery(const std::string &sql)
{
    if (!isConnected() && !connect())
        return false;

    PGresult *res = PQexec(connection, sql.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(connection) << std::endl;
        PQclear(res);
        return false;
    }

    PQclear(res);
    return true;
}

PGresult *Database::executeSelect(const std::string &sql)
{
    if (!isConnected() && !connect())
        return nullptr;

    PGresult *res = PQexec(connection, sql.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::cerr << "Ошибка SELECT: " << PQerrorMessage(connection) << std::endl;
        PQclear(res);
        return nullptr;
    }

    return res;
}

User *Database::getUserByLogin(const std::string &login)
{
    std::string sql = "SELECT id, login, password, name, role FROM users WHERE login = '" + login + "'";
    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return nullptr;
    }

    int id = std::stoi(PQgetvalue(res, 0, 0));
    std::string userLogin = PQgetvalue(res, 0, 1);
    std::string password = PQgetvalue(res, 0, 2);
    std::string name = PQgetvalue(res, 0, 3);
    std::string roleStr = PQgetvalue(res, 0, 4);

    PQclear(res);

    std::cout << "User found: " << userLogin << ", role: " << roleStr << std::endl; // Отладка

    Role role = (roleStr == "admin") ? Role::Admin : Role::Employee;

    if (role == Role::Admin)
    {
        return new Admin(id, userLogin, password, name);
    }
    else
    {
        return new Employee(id, userLogin, password, name);
    }
}

bool Database::saveUser(const std::string &login, const std::string &password, const std::string &name, const std::string &role)
{
    std::string sql = "INSERT INTO users (login, password, name, role) VALUES ('" +
                      login + "', '" + password + "', '" + name + "', '" + role + "')";
    return executeQuery(sql);
}

std::vector<Product> Database::getAllProducts()
{
    std::vector<Product> products;
    PGresult *res = executeSelect("SELECT id, sku, name, price, description FROM products ORDER BY id");

    if (!res)
        return products;

    for (int i = 0; i < PQntuples(res); ++i)
    {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string sku = PQgetvalue(res, i, 1);
        std::string name = PQgetvalue(res, i, 2);
        double price = std::stod(PQgetvalue(res, i, 3));
        std::string desc = PQgetvalue(res, i, 4);

        products.emplace_back(id, sku, name, price, desc);
    }

    PQclear(res);
    return products;
}

Product *Database::getProductById(int id)
{
    std::string sql = "SELECT id, sku, name, price, description FROM products WHERE id = " + std::to_string(id);
    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return nullptr;
    }

    int prodId = std::stoi(PQgetvalue(res, 0, 0));
    std::string sku = PQgetvalue(res, 0, 1);
    std::string name = PQgetvalue(res, 0, 2);
    double price = std::stod(PQgetvalue(res, 0, 3));
    std::string desc = PQgetvalue(res, 0, 4);

    PQclear(res);
    return new Product(prodId, sku, name, price, desc);
}

Product *Database::getProductBySku(const std::string &sku)
{
    std::string sql = "SELECT id, sku, name, price, description FROM products WHERE sku = '" + sku + "'";
    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return nullptr;
    }

    int id = std::stoi(PQgetvalue(res, 0, 0));
    std::string productSku = PQgetvalue(res, 0, 1);
    std::string name = PQgetvalue(res, 0, 2);
    double price = std::stod(PQgetvalue(res, 0, 3));
    std::string desc = PQgetvalue(res, 0, 4);

    PQclear(res);
    return new Product(id, productSku, name, price, desc);
}

bool Database::addProduct(const Product &product)
{
    std::string sql = "INSERT INTO products (sku, name, price, description) VALUES ('" +
                      product.getSku() + "', '" + product.getName() + "', " +
                      formatDouble(product.getPrice()) + ", '" + product.getDescription() + "')";

    if (executeQuery(sql))
    {
        // Добавляем остатки для нового товара
        return executeQuery("INSERT INTO stocks (product_id, quantity) VALUES ((SELECT lastval()), 0)");
    }
    return false;
}

bool Database::updateProduct(const Product &product)
{
    std::string sql = "UPDATE products SET name = '" + product.getName() +
                      "', price = " + formatDouble(product.getPrice()) +
                      ", description = '" + product.getDescription() +
                      "' WHERE id = " + std::to_string(product.getId());
    return executeQuery(sql);
}

bool Database::deleteProduct(int id)
{
    executeQuery("DELETE FROM order_items WHERE product_id = " + std::to_string(id));
    executeQuery("DELETE FROM receipt_items WHERE product_id = " + std::to_string(id));
    executeQuery("DELETE FROM shipment_items WHERE product_id = " + std::to_string(id));
    executeQuery("DELETE FROM stocks WHERE product_id = " + std::to_string(id));
    return executeQuery("DELETE FROM products WHERE id = " + std::to_string(id));
}

Stock *Database::getStock(int productId)
{
    std::string sql = "SELECT product_id, quantity, min_threshold FROM stocks WHERE product_id = " + std::to_string(productId);
    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return nullptr;
    }

    int prodId = std::stoi(PQgetvalue(res, 0, 0));
    int quantity = std::stoi(PQgetvalue(res, 0, 1));
    int threshold = std::stoi(PQgetvalue(res, 0, 2));

    PQclear(res);
    return new Stock(prodId, quantity, threshold);
}

bool Database::updateStock(int productId, int quantity)
{
    std::string sql = "UPDATE stocks SET quantity = " + std::to_string(quantity) +
                      ", updated_at = CURRENT_TIMESTAMP WHERE product_id = " + std::to_string(productId);
    return executeQuery(sql);
}

std::vector<Stock> Database::getAllStocks()
{
    std::vector<Stock> stocks;
    PGresult *res = executeSelect("SELECT product_id, quantity, min_threshold FROM stocks ORDER BY product_id");

    if (!res)
        return stocks;

    for (int i = 0; i < PQntuples(res); ++i)
    {
        int productId = std::stoi(PQgetvalue(res, i, 0));
        int quantity = std::stoi(PQgetvalue(res, i, 1));
        int threshold = std::stoi(PQgetvalue(res, i, 2));

        stocks.emplace_back(productId, quantity, threshold);
    }

    PQclear(res);
    return stocks;
}

std::vector<Order> Database::getAllOrders()
{
    std::vector<Order> orders;
    PGresult *res = executeSelect("SELECT id, order_number, date, status, employee_id FROM orders ORDER BY id DESC");

    if (!res)
    {
        qDebug() << "getAllOrders: no result";
        return orders;
    }

    qDebug() << "getAllOrders: rows =" << PQntuples(res);

    for (int i = 0; i < PQntuples(res); ++i)
    {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string number = PQgetvalue(res, i, 1);
        std::string date = PQgetvalue(res, i, 2);
        std::string status = PQgetvalue(res, i, 3);
        int employeeId = std::stoi(PQgetvalue(res, i, 4));

        Order order(id, number, employeeId);
        order.setDate(date);
        order.setStatus(status);

        orders.push_back(order);
    }

    PQclear(res);
    qDebug() << "getAllOrders: returning" << orders.size() << "orders";
    return orders;
}
Order *Database::getOrderById(int id)
{
    std::string sql = "SELECT id, order_number, date, status, employee_id FROM orders WHERE id = " + std::to_string(id);
    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return nullptr;
    }

    int orderId = std::stoi(PQgetvalue(res, 0, 0));
    std::string number = PQgetvalue(res, 0, 1);
    std::string date = PQgetvalue(res, 0, 2);
    std::string status = PQgetvalue(res, 0, 3);
    int employeeId = std::stoi(PQgetvalue(res, 0, 4));

    PQclear(res);

    Order *order = new Order(orderId, number, employeeId);
    order->setDate(date);
    order->setStatus(status);

    // ЗАГРУЖАЕМ ПОЗИЦИИ ЗАКАЗА
    std::string itemsSql = "SELECT product_id, quantity, price FROM order_items WHERE order_id = " + std::to_string(orderId);
    PGresult *itemsRes = executeSelect(itemsSql);

    if (itemsRes)
    {
        for (int i = 0; i < PQntuples(itemsRes); ++i)
        {
            int productId = std::stoi(PQgetvalue(itemsRes, i, 0));
            int quantity = std::stoi(PQgetvalue(itemsRes, i, 1));
            double price = std::stod(PQgetvalue(itemsRes, i, 2));

            Product *product = getProductById(productId);
            if (product)
            {
                order->addItem(OrderItem(product, quantity, price));
                qDebug() << "Loaded order item: productId=" << productId << "qty=" << quantity;
            }
        }
        PQclear(itemsRes);
    }

    qDebug() << "Order" << orderId << "has" << order->getItems().size() << "items";
    return order;
}

int Database::saveOrder(const Order &order)
{
    qDebug() << "=== saveOrder START ===";
    qDebug() << "Order has" << order.getItems().size() << "items";

    std::string sql;
    int id = 0;

    if (order.getId() == 0)
    {
        sql = "INSERT INTO orders (order_number, employee_id, status) VALUES ('" +
              order.getNumber() + "', " + std::to_string(order.getEmployeeId()) + ", 'Создан') RETURNING id";
    }
    else
    {
        sql = "UPDATE orders SET status = '" + order.getStatus() +
              "', total = " + formatDouble(order.getTotalPrice()) +
              " WHERE id = " + std::to_string(order.getId()) + " RETURNING id";
    }

    PGresult *res = executeSelect(sql);
    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        qDebug() << "saveOrder: failed to save order";
        return 0;
    }

    id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    qDebug() << "Order saved with id=" << id;
    qDebug() << "Saving" << order.getItems().size() << "order items...";
    for (const auto &item : order.getItems())
    {
        qDebug() << "  Item: productId=" << item.getProductId()
                 << "quantity=" << item.getQuantity()
                 << "price=" << item.getPrice();

        std::string itemSql = "INSERT INTO order_items (order_id, product_id, quantity, price) VALUES (" +
                              std::to_string(id) + ", " + std::to_string(item.getProductId()) + ", " +
                              std::to_string(item.getQuantity()) + ", " + formatDouble(item.getPrice()) + ")";
        if (!executeQuery(itemSql))
        {
            qDebug() << "saveOrder: FAILED to save order item!";
        }
        else
        {
            qDebug() << "  Item saved successfully";
        }
    }
    std::string updateTotalSql = "UPDATE orders SET total = " + formatDouble(order.getTotalPrice()) +
                                 " WHERE id = " + std::to_string(id);
    executeQuery(updateTotalSql);

    qDebug() << "=== saveOrder END ===";
    return id;
}
bool Database::updateOrderStatus(int id, const std::string &status)
{
    return executeQuery("UPDATE orders SET status = '" + status + "' WHERE id = " + std::to_string(id));
}

int Database::saveReceipt(const Receipt &receipt)
{
    std::string sql;

    if (receipt.getId() == 0)
    {
        std::string receiptNumber = receipt.getNumber();
        if (receiptNumber.empty())
        {
            receiptNumber = "RCP-" + std::to_string(std::time(nullptr));
        }
        sql = "INSERT INTO receipts (receipt_number, supplier, employee_id) VALUES ('" +
              receiptNumber + "', '" + receipt.getSupplier() + "', " +
              std::to_string(receipt.getEmployeeId()) + ") RETURNING id";
    }
    else
    {
        sql = "UPDATE receipts SET supplier = '" + receipt.getSupplier() +
              "' WHERE id = " + std::to_string(receipt.getId()) + " RETURNING id";
    }

    PGresult *res = executeSelect(sql);
    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return 0;
    }

    int id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    for (const auto &item : receipt.getItems())
    {
        std::string itemSql = "INSERT INTO receipt_items (receipt_id, product_id, quantity, buy_price) VALUES (" +
                              std::to_string(id) + ", " + std::to_string(item.getProductId()) + ", " +
                              std::to_string(item.getQuantity()) + ", " + std::to_string(item.getBuyPrice()) + ")";
        executeQuery(itemSql);
    }

    return id;
}

std::vector<Receipt> Database::getAllReceipts()
{
    std::vector<Receipt> receipts;
    PGresult *res = executeSelect("SELECT id, receipt_number, date, supplier, employee_id FROM receipts ORDER BY id DESC");

    if (!res)
        return receipts;

    for (int i = 0; i < PQntuples(res); ++i)
    {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string number = PQgetvalue(res, i, 1);
        std::string date = PQgetvalue(res, i, 2);
        std::string supplier = PQgetvalue(res, i, 3);
        int employeeId = std::stoi(PQgetvalue(res, i, 4));

        Receipt receipt(id, number, date, supplier, employeeId);
        receipts.push_back(receipt);
    }

    PQclear(res);
    return receipts;
}

int Database::saveShipment(const Shipment &shipment)
{
    std::string sql;

    if (shipment.getId() == 0)
    {
        sql = "INSERT INTO shipments (shipment_number, order_id, employee_id) VALUES ('" +
              shipment.getNumber() + "', " + std::to_string(shipment.getOrderId()) + ", " +
              std::to_string(shipment.getEmployeeId()) + ") RETURNING id";
    }
    else
    {
        sql = "UPDATE shipments SET order_id = " + std::to_string(shipment.getOrderId()) +
              " WHERE id = " + std::to_string(shipment.getId()) + " RETURNING id";
    }

    PGresult *res = executeSelect(sql);

    if (!res || PQntuples(res) == 0)
    {
        if (res)
            PQclear(res);
        return 0;
    }

    int id = std::stoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    // Сохраняем позиции расхода
    for (const auto &item : shipment.getItems())
    {
        std::string itemSql = "INSERT INTO shipment_items (shipment_id, product_id, quantity, sell_price) VALUES (" +
                              std::to_string(id) + ", " + std::to_string(item.getProductId()) + ", " +
                              std::to_string(item.getQuantity()) + ", " + std::to_string(item.getSellPrice()) + ")";
        executeQuery(itemSql);
    }

    return id;
}

std::vector<Shipment> Database::getAllShipments()
{
    std::vector<Shipment> shipments;
    PGresult *res = executeSelect("SELECT id, shipment_number, date, order_id, employee_id FROM shipments ORDER BY id DESC");

    if (!res)
        return shipments;

    for (int i = 0; i < PQntuples(res); ++i)
    {
        int id = std::stoi(PQgetvalue(res, i, 0));
        std::string number = PQgetvalue(res, i, 1);
        std::string date = PQgetvalue(res, i, 2);
        int orderId = std::stoi(PQgetvalue(res, i, 3));
        int employeeId = std::stoi(PQgetvalue(res, i, 4));

        Shipment shipment(id, number, date, orderId, employeeId);
        shipments.push_back(shipment);
    }

    PQclear(res);
    return shipments;
}

bool Database::logAction(int userId, const std::string &action, const std::string &details)
{
    std::string sql = "INSERT INTO audit_log (user_id, action, details) VALUES (" +
                      std::to_string(userId) + ", '" + action + "', '" + details + "')";
    return executeQuery(sql);
}