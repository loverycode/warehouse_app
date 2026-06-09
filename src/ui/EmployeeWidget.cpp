#include "ui/EmployeeWidget.h"
#include "ui/StockWidget.h"
#include "ui/OrderWidget.h"
#include "ui/ReceiptWidget.h"
#include <QDebug>

EmployeeWidget::EmployeeWidget(QWidget *parent) : QTabWidget(parent)
{
    setWindowTitle("Система управления складом - Сотрудник");
    resize(1000, 700);

    qDebug() << "=== СОЗДАНИЕ ОКНА СОТРУДНИКА ===";

    qDebug() << "1. Создаём StockWidget...";
    StockWidget *stockWidget = new StockWidget(this);
    qDebug() << "   StockWidget создан";

    qDebug() << "2. Создаём OrderWidget...";
    OrderWidget *orderWidget = new OrderWidget(this);
    qDebug() << "   OrderWidget создан";

    qDebug() << "3. Создаём ReceiptWidget...";
    ReceiptWidget *receiptWidget = new ReceiptWidget(this);
    qDebug() << "   ReceiptWidget создан";

    qDebug() << "4. Добавляем вкладки...";
    addTab(stockWidget, "Остатки");
    addTab(orderWidget, "Заказы");
    addTab(receiptWidget, "Приход товара");

    qDebug() << "=== ОКНО СОТРУДНИКА ГОТОВО ===";
}

EmployeeWidget::~EmployeeWidget() {}