#include "ui/StockWidget.h"
#include "database/Database.h"
#include "core/Product.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QHeaderView>

StockWidget::StockWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"ID", "Артикул", "Товар", "Количество"});
    table->horizontalHeader()->setStretchLastSection(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    refreshButton = new QPushButton("Обновить", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshButton);

    layout->addWidget(table);
    layout->addLayout(buttonLayout);

    connect(refreshButton, &QPushButton::clicked, this, &StockWidget::refreshTable);

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &StockWidget::refreshTable);
    refreshTimer->start(5000); // Обновление каждые 5 секунд

    refreshTable();
}

void StockWidget::refreshTable()
{
    Database *db = Database::getInstance();
    auto stocks = db->getAllStocks();

    table->setRowCount(stocks.size());

    int row = 0;
    for (const auto &stock : stocks)
    {
        Product *product = db->getProductById(stock.getProductId());
        if (product)
        {
            table->setItem(row, 0, new QTableWidgetItem(QString::number(product->getId())));
            table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(product->getSku())));
            table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(product->getName())));
            table->setItem(row, 3, new QTableWidgetItem(QString::number(stock.getQuantity())));

            // Подсветка критического остатка
            if (stock.getQuantity() < 5)
            {
                for (int col = 0; col < 4; ++col)
                {
                    QTableWidgetItem *item = table->item(row, col);
                    if (item)
                    {
                        item->setBackground(Qt::red);
                        item->setForeground(Qt::white);
                    }
                }
            }
        }
        ++row;
    }

    table->resizeColumnsToContents();
}