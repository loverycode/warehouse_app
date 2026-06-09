#ifndef RECEIPT_WIDGET_H
#define RECEIPT_WIDGET_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QComboBox;
class QLineEdit;
class QSpinBox;

class ReceiptWidget : public QWidget
{
public:
    ReceiptWidget(QWidget *parent = nullptr);
private slots:
    void onAddItem();
    void onSaveReceipt();

private:
    QTableWidget *itemsTable;
    QComboBox *productCombo;
    QSpinBox *quantitySpin;
    QLineEdit *supplierEdit;
    QPushButton *addButton;
    QPushButton *saveButton;
    std::vector<std::pair<int, int>> items; // productId, quantity
    void refreshItemsTable();
};

#endif