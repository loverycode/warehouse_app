#ifndef ORDER_WIDGET_H
#define ORDER_WIDGET_H

#include <QWidget>
#include <QDialog>

class QTableWidget;
class QPushButton;
class QComboBox;
class QSpinBox;

class CreateOrderDialog : public QDialog
{
public:
    CreateOrderDialog(QWidget *parent = nullptr);
    ~CreateOrderDialog();
    int getProductId() const;
    int getQuantity() const;

private:
    QComboBox *productCombo;
    QSpinBox *quantitySpin;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

class OrderWidget : public QWidget
{
public:
    OrderWidget(QWidget *parent = nullptr);
    ~OrderWidget();

public slots:
    void refreshTable();
    void onCreateOrder();
    void onShipOrder();
    void onCancelOrder();

private:
    QTableWidget *table;
    QPushButton *createButton;
    QPushButton *shipButton;
    QPushButton *refreshButton;
    QPushButton *cancelButton;
    int selectedOrderId;
};

#endif