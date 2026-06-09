#include "ui/ProductWidget.h"
#include "database/Database.h"
#include "core/Product.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>

// ============================================
// ДИАЛОГ ДОБАВЛЕНИЯ ТОВАРА
// ============================================
class AddProductDialog : public QDialog
{
public:
    AddProductDialog(QWidget *parent = nullptr);
    std::string getSku() const { return skuEdit->text().toStdString(); }
    std::string getName() const { return nameEdit->text().toStdString(); }
    double getPrice() const { return priceSpin->value(); }
    std::string getDescription() const { return descEdit->text().toStdString(); }

private:
    QLineEdit *skuEdit;
    QLineEdit *nameEdit;
    QDoubleSpinBox *priceSpin;
    QLineEdit *descEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

AddProductDialog::AddProductDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Добавление товара");
    setModal(true);
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Артикул (SKU):"));
    skuEdit = new QLineEdit(this);
    layout->addWidget(skuEdit);

    layout->addWidget(new QLabel("Название:"));
    nameEdit = new QLineEdit(this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Цена:"));
    priceSpin = new QDoubleSpinBox(this);
    priceSpin->setMinimum(0);
    priceSpin->setMaximum(100000);
    priceSpin->setValue(0);
    layout->addWidget(priceSpin);

    layout->addWidget(new QLabel("Описание:"));
    descEdit = new QLineEdit(this);
    layout->addWidget(descEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("Добавить");
    cancelButton = new QPushButton("Отмена");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

// ============================================
// ДИАЛОГ РЕДАКТИРОВАНИЯ ТОВАРА
// ============================================
class EditProductDialog : public QDialog
{
public:
    EditProductDialog(Product *product, QWidget *parent = nullptr);
    std::string getName() const { return nameEdit->text().toStdString(); }
    double getPrice() const { return priceSpin->value(); }
    std::string getDescription() const { return descEdit->text().toStdString(); }

private:
    QLineEdit *nameEdit;
    QDoubleSpinBox *priceSpin;
    QLineEdit *descEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

EditProductDialog::EditProductDialog(Product *product, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Редактирование товара");
    setModal(true);
    resize(400, 250);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Название:"));
    nameEdit = new QLineEdit(QString::fromStdString(product->getName()), this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Цена:"));
    priceSpin = new QDoubleSpinBox(this);
    priceSpin->setMinimum(0);
    priceSpin->setMaximum(100000);
    priceSpin->setValue(product->getPrice());
    layout->addWidget(priceSpin);

    layout->addWidget(new QLabel("Описание:"));
    descEdit = new QLineEdit(QString::fromStdString(product->getDescription()), this);
    layout->addWidget(descEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    okButton = new QPushButton("Сохранить");
    cancelButton = new QPushButton("Отмена");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

// ============================================
// PRODUCT WIDGET
// ============================================
ProductWidget::ProductWidget(QWidget *parent) : QWidget(parent), selectedProductId(-1)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID", "Артикул", "Название", "Цена", "Описание"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("Добавить", this);
    editButton = new QPushButton("Редактировать", this);
    deleteButton = new QPushButton("Удалить", this);
    refreshButton = new QPushButton("Обновить", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshButton);

    layout->addWidget(table);
    layout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &ProductWidget::onAddProduct);
    connect(editButton, &QPushButton::clicked, this, &ProductWidget::onEditProduct);
    connect(deleteButton, &QPushButton::clicked, this, &ProductWidget::onDeleteProduct);
    connect(refreshButton, &QPushButton::clicked, this, &ProductWidget::refreshTable);

    connect(table, &QTableWidget::itemClicked, this, [this](QTableWidgetItem *item)
            {
        if (item) {
            selectedProductId = table->item(item->row(), 0)->text().toInt();
        } });

    refreshTable();
}

void ProductWidget::refreshTable()
{
    Database *db = Database::getInstance();
    auto products = db->getAllProducts();

    table->setRowCount(products.size());

    int row = 0;
    for (const auto &product : products)
    {
        table->setItem(row, 0, new QTableWidgetItem(QString::number(product.getId())));
        table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(product.getSku())));
        table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(product.getName())));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(product.getPrice())));
        table->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(product.getDescription())));
        ++row;
    }

    table->resizeColumnsToContents();
    selectedProductId = -1;
}

void ProductWidget::onAddProduct()
{
    AddProductDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Database *db = Database::getInstance();
        Product newProduct(0, dialog.getSku(), dialog.getName(), dialog.getPrice(), dialog.getDescription());

        if (db->addProduct(newProduct))
        {
            refreshTable();
            QMessageBox::information(this, "Успех", "Товар добавлен!");
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить товар");
        }
    }
}

void ProductWidget::onEditProduct()
{
    if (selectedProductId == -1)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для редактирования!");
        return;
    }

    Database *db = Database::getInstance();
    Product *product = db->getProductById(selectedProductId);

    if (!product)
    {
        QMessageBox::warning(this, "Ошибка", "Товар не найден!");
        return;
    }

    EditProductDialog dialog(product, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Product updatedProduct(product->getId(), product->getSku(), dialog.getName(), dialog.getPrice(), dialog.getDescription());
        if (db->updateProduct(updatedProduct))
        {
            refreshTable();
            QMessageBox::information(this, "Успех", "Товар обновлён!");
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось обновить товар");
        }
    }
}

void ProductWidget::onDeleteProduct()
{
    if (selectedProductId == -1)
    {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Удалить товар? Это действие нельзя отменить.",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        Database *db = Database::getInstance();
        if (db->deleteProduct(selectedProductId))
        {
            refreshTable();
            QMessageBox::information(this, "Успех", "Товар удалён!");
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить товар");
        }
    }
}