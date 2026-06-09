#include "ui/ReceiptWidget.h"
#include "database/Database.h"
#include "core/Product.h"
#include "documents/Receipt.h"
#include "documents/ReceiptItem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

ReceiptWidget::ReceiptWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Форма добавления товара
    QHBoxLayout *addLayout = new QHBoxLayout();
    addLayout->addWidget(new QLabel("Товар:"));
    productCombo = new QComboBox(this);
    auto products = Database::getInstance()->getAllProducts();
    for (const auto &product : products)
    {
        productCombo->addItem(QString::fromStdString(product.getName()), product.getId());
    }
    addLayout->addWidget(productCombo);

    addLayout->addWidget(new QLabel("Количество:"));
    quantitySpin = new QSpinBox(this);
    quantitySpin->setMinimum(1);
    quantitySpin->setMaximum(999);
    addLayout->addWidget(quantitySpin);

    addButton = new QPushButton("Добавить", this);
    addLayout->addWidget(addButton);
    addLayout->addStretch();

    mainLayout->addLayout(addLayout);

    // Таблица добавленных товаров
    itemsTable = new QTableWidget(this);
    itemsTable->setColumnCount(3);
    itemsTable->setHorizontalHeaderLabels({"Товар", "Количество", "Действия"});
    itemsTable->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(itemsTable);

    // Поставщик
    QHBoxLayout *supplierLayout = new QHBoxLayout();
    supplierLayout->addWidget(new QLabel("Поставщик:"));
    supplierEdit = new QLineEdit(this);
    supplierLayout->addWidget(supplierEdit);
    supplierLayout->addStretch();
    mainLayout->addLayout(supplierLayout);

    // Кнопка сохранения
    saveButton = new QPushButton("Сохранить приход", this);
    mainLayout->addWidget(saveButton);

    connect(addButton, &QPushButton::clicked, this, &ReceiptWidget::onAddItem);
    connect(saveButton, &QPushButton::clicked, this, &ReceiptWidget::onSaveReceipt);
}

void ReceiptWidget::onAddItem()
{
    int productId = productCombo->currentData().toInt();
    int quantity = quantitySpin->value();

    items.push_back({productId, quantity});
    refreshItemsTable();

    quantitySpin->setValue(1);
}

void ReceiptWidget::refreshItemsTable()
{
    Database *db = Database::getInstance();
    itemsTable->setRowCount(items.size());

    int row = 0;
    for (const auto &item : items)
    {
        Product *product = db->getProductById(item.first);
        itemsTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(product->getName())));
        itemsTable->setItem(row, 1, new QTableWidgetItem(QString::number(item.second)));

        QPushButton *removeButton = new QPushButton("Удалить");
        removeButton->setProperty("row", row);
        connect(removeButton, &QPushButton::clicked, [this, row]()
                {
            items.erase(items.begin() + row);
            refreshItemsTable(); });
        itemsTable->setCellWidget(row, 2, removeButton);

        ++row;
    }

    itemsTable->resizeColumnsToContents();
}

void ReceiptWidget::onSaveReceipt()
{
    if (items.empty())
    {
        QMessageBox::warning(this, "Ошибка", "Добавьте хотя бы один товар!");
        return;
    }

    if (supplierEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Укажите поставщика!");
        return;
    }

    Database *db = Database::getInstance();

    Receipt receipt;
    receipt.setSupplier(supplierEdit->text().toStdString());
    receipt.setEmployeeId(1); // TODO: реальный ID сотрудника

    for (const auto &item : items)
    {
        Product *product = db->getProductById(item.first);
        receipt.addItem(ReceiptItem(product, item.second));
    }

    receipt.save();

    items.clear();
    supplierEdit->clear();
    refreshItemsTable();

    QMessageBox::information(this, "Успех", "Приход успешно оформлен!");
}