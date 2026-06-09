#ifndef STOCK_WIDGET_H
#define STOCK_WIDGET_H

#include <QWidget>
#include <QTimer>

class QTableWidget;
class QPushButton;

class StockWidget : public QWidget
{
public:
    StockWidget(QWidget *parent = nullptr);
private slots:
    void refreshTable();

private:
    QTableWidget *table;
    QPushButton *refreshButton;
    QTimer *refreshTimer;
};

#endif