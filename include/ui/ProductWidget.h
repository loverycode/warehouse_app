#ifndef PRODUCT_WIDGET_H
#define PRODUCT_WIDGET_H

#include <QWidget>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QDoubleSpinBox;

class ProductWidget : public QWidget
{
public:
    ProductWidget(QWidget *parent = nullptr);
private slots:
    void refreshTable();
    void onAddProduct();
    void onEditProduct();
    void onDeleteProduct();

private:
    QTableWidget *table;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    int selectedProductId;
};

#endif