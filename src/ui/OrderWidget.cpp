#include "ui/OrderWidget.h"
#include "database/Database.h"
#include "core/Product.h"
#include "documents/Order.h"
#include "documents/OrderItem.h"
#include "documents/Shipment.h"
#include "documents/ShipmentItem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QComboBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

// ============================================
// ДИАЛОГ СОЗДАНИЯ ЗАКАЗА
// ============================================
CreateOrderDialog::CreateOrderDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Создание заказа");
    setModal(true);
    resize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *productLayout = new QHBoxLayout();
    productLayout->addWidget(new QLabel("Товар:"));
    productCombo = new QComboBox(this);
    productLayout->addWidget(productCombo);
    layout->addLayout(productLayout);

    QHBoxLayout *quantityLayout = new QHBoxLayout();
    quantityLayout->addWidget(new QLabel("Количество:"));
    quantitySpin = new QSpinBox(this);
    quantitySpin->setMinimum(1);
    quantitySpin->setMaximum(999);
    quantityLayout->addWidget(quantitySpin);
    layout->addLayout(quantityLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("Создать", this);
    cancelButton = new QPushButton("Отмена", this);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    Database *db = Database::getInstance();
    auto products = db->getAllProducts();
    for (const auto &product : products)
    {
        productCombo->addItem(QString::fromStdString(product.getName()), product.getId());
    }
}

int CreateOrderDialog::getProductId() const
{
    return productCombo->currentData().toInt();
}

int CreateOrderDialog::getQuantity() const
{
    return quantitySpin->value();
}

CreateOrderDialog::~CreateOrderDialog() {}

// ============================================
// ORDER WIDGET
// ============================================
OrderWidget::OrderWidget(QWidget *parent) : QWidget(parent), selectedOrderId(-1)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Номер", "Дата", "Статус", "Действия"});
    table->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    createButton = new QPushButton("Создать заказ", this);
    shipButton = new QPushButton("Отгрузить", this);
    cancelButton = new QPushButton("Отменить заказ", this);
    refreshButton = new QPushButton("Обновить", this);

    buttonLayout->addWidget(createButton);
    buttonLayout->addWidget(shipButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshButton);

    layout->addWidget(table);
    layout->addLayout(buttonLayout);

    connect(createButton, &QPushButton::clicked, this, &OrderWidget::onCreateOrder);
    connect(shipButton, &QPushButton::clicked, this, &OrderWidget::onShipOrder);
    connect(cancelButton, &QPushButton::clicked, this, &OrderWidget::onCancelOrder);
    connect(refreshButton, &QPushButton::clicked, this, &OrderWidget::refreshTable);
}

void OrderWidget::refreshTable()
{
    qDebug() << "refreshTable() started";

    Database *db = Database::getInstance();
    if (!db || !db->isConnected())
    {
        qDebug() << "Database not connected";
        return;
    }

    std::vector<Order> orders = db->getAllOrders();
    qDebug() << "Got" << orders.size() << "orders";

    table->clear();
    table->setRowCount(0);
    table->setColumnCount(5); // 5 колонок
    table->setHorizontalHeaderLabels({"ID", "Номер", "Дата", "Статус", "Действия"});
    table->horizontalHeader()->setStretchLastSection(true);

    table->setRowCount(orders.size());

    for (int row = 0; row < (int)orders.size(); ++row)
    {
        const Order &order = orders[row];

        QTableWidgetItem *idItem = new QTableWidgetItem();
        idItem->setData(Qt::DisplayRole, order.getId());
        table->setItem(row, 0, idItem);

        QTableWidgetItem *numberItem = new QTableWidgetItem();
        numberItem->setText(QString::fromStdString(order.getNumber()));
        table->setItem(row, 1, numberItem);

        QTableWidgetItem *dateItem = new QTableWidgetItem();
        dateItem->setText(QString::fromStdString(order.getDate()));
        table->setItem(row, 2, dateItem);

        QTableWidgetItem *statusItem = new QTableWidgetItem();
        statusItem->setText(QString::fromStdString(order.getStatus()));
        table->setItem(row, 3, statusItem);

        // Подсветка статуса
        if (order.getStatus() == "Закрыт")
        {
            statusItem->setBackground(Qt::green);
            statusItem->setForeground(Qt::black);
        }
        else if (order.getStatus() == "Отменён")
        {
            statusItem->setBackground(Qt::red);
            statusItem->setForeground(Qt::white);
        }

        // Кнопка выбора (без модалки, только подсветка)
        QPushButton *selectButton = new QPushButton("Выбрать");
        selectButton->setProperty("row", row);
        selectButton->setProperty("orderId", order.getId());
        connect(selectButton, &QPushButton::clicked, [this, row, orderId = order.getId()]()
                {
            // Снимаем подсветку со всех строк
            for (int r = 0; r < table->rowCount(); ++r)
            {
                table->item(r, 0)->setBackground(Qt::transparent);
                table->item(r, 1)->setBackground(Qt::transparent);
                table->item(r, 2)->setBackground(Qt::transparent);
                table->item(r, 3)->setBackground(Qt::transparent);
            }
            // Подсвечиваем выбранную строку
            for (int col = 0; col < 4; ++col)  // 4 колонки для подсветки (без колонки действий)
            {
                QTableWidgetItem *item = table->item(row, col);
                if (item)
                {
                    item->setBackground(Qt::yellow);
                }
            }
            selectedOrderId = orderId; });
        table->setCellWidget(row, 4, selectButton); // колонка 4 = "Действия"

        // Если это выбранный заказ, подсвечиваем его
        if (order.getId() == selectedOrderId)
        {
            for (int col = 0; col < 4; ++col)
            {
                QTableWidgetItem *item = table->item(row, col);
                if (item)
                {
                    item->setBackground(Qt::yellow);
                }
            }
        }
    }

    table->resizeColumnsToContents();
    qDebug() << "refreshTable() finished";
}

void OrderWidget::onCreateOrder()
{
    qDebug() << "=== onCreateOrder START ===";

    CreateOrderDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        int productId = dialog.getProductId();
        int quantity = dialog.getQuantity();

        Database *db = Database::getInstance();
        Product *product = db->getProductById(productId);
        Stock *stock = db->getStock(productId);

        if (!stock || stock->getQuantity() < quantity)
        {
            QMessageBox::warning(this, "Ошибка", "Недостаточно товара на складе!");
            return;
        }

        Order order;
        order.setNumber("ORD-" + QString::number(QDateTime::currentSecsSinceEpoch()).toStdString());
        order.addItem(OrderItem(product, quantity));
        order.setEmployeeId(1);

        int orderId = db->saveOrder(order);
        qDebug() << "Order saved with ID:" << orderId;

        if (orderId > 0)
        {
            refreshTable();
            QMessageBox::information(this, "Успех", "Заказ успешно создан!");
        }
    }
    qDebug() << "=== onCreateOrder END ===";
}

void OrderWidget::onShipOrder()
{
    qDebug() << "=== onShipOrder START ===";
    qDebug() << "selectedOrderId:" << selectedOrderId;

    if (selectedOrderId == -1)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите заказ для отгрузки!");
        return;
    }

    Database *db = Database::getInstance();
    Order *order = db->getOrderById(selectedOrderId);

    if (!order)
    {
        QMessageBox::warning(this, "Ошибка", "Заказ не найден!");
        return;
    }

    if (order->getStatus() != "Создан")
    {
        QString msg = "Заказ не может быть отгружен!\nТекущий статус: " + QString::fromStdString(order->getStatus());
        QMessageBox::warning(this, "Ошибка", msg);
        return;
    }

    if (order->getItems().empty())
    {
        QMessageBox::warning(this, "Ошибка", "В заказе нет товаров!");
        return;
    }

    for (const auto &item : order->getItems())
    {
        Stock *stock = db->getStock(item.getProductId());
        if (!stock || stock->getQuantity() < item.getQuantity())
        {
            Product *product = db->getProductById(item.getProductId());
            QMessageBox::warning(this, "Ошибка",
                                 QString("Недостаточно товара: %1").arg(QString::fromStdString(product->getName())));
            return;
        }
    }

    Shipment shipment;
    shipment.setNumber("SHP-" + QString::number(QDateTime::currentSecsSinceEpoch()).toStdString());
    shipment.setOrderId(selectedOrderId);
    shipment.setEmployeeId(1);

    for (const auto &item : order->getItems())
    {
        Product *product = db->getProductById(item.getProductId());
        if (product)
        {
            shipment.addItem(ShipmentItem(product, item.getQuantity()));
        }
    }
    shipment.save();

    refreshTable();
    selectedOrderId = -1;
    QMessageBox::information(this, "Успех", "Заказ успешно отгружен!");
    qDebug() << "=== onShipOrder END ===";
}

void OrderWidget::onCancelOrder()
{
    if (selectedOrderId == -1)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите заказ для отмены!");
        return;
    }

    Database *db = Database::getInstance();
    Order *order = db->getOrderById(selectedOrderId);

    if (!order)
    {
        QMessageBox::warning(this, "Ошибка", "Заказ не найден!");
        return;
    }

    if (order->getStatus() != "Создан")
    {
        QMessageBox::warning(this, "Ошибка", "Можно отменить только заказ в статусе 'Создан'!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Вы уверены, что хотите отменить заказ №" + QString::number(selectedOrderId) + "?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        db->updateOrderStatus(selectedOrderId, "Отменён");
        refreshTable();
        selectedOrderId = -1;
        QMessageBox::information(this, "Успех", "Заказ отменён!");
    }
}

OrderWidget::~OrderWidget() {}