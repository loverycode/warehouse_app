#include "ui/AdminWidget.h"
#include "ui/StockWidget.h"
#include "ui/ProductWidget.h"
#include "ui/ReportWidget.h"

AdminWidget::AdminWidget(QWidget *parent) : QTabWidget(parent)
{
    setWindowTitle("Система управления складом - Администратор");
    resize(1000, 700);

    addTab(new StockWidget(this), "Остатки");
    addTab(new ProductWidget(this), "Товары");
    addTab(new ReportWidget(this), "Отчёты");
}