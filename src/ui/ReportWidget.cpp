#include "ui/ReportWidget.h"
#include "database/Database.h"
#include "core/Product.h"
#include "documents/Receipt.h"
#include "documents/Shipment.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDateEdit>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>

ReportWidget::ReportWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Верхняя панель с кнопками
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    stockReportButton = new QPushButton("Отчёт по остаткам", this);
    movementReportButton = new QPushButton("Отчёт по движению", this);
    exportJsonButton = new QPushButton("Экспорт JSON", this);
    exportPdfButton = new QPushButton("Экспорт PDF", this);

    // Устанавливаем стиль кнопок
    QString buttonStyle = "QPushButton { padding: 8px; border-radius: 5px; }"
                          "QPushButton:hover { background-color: #e0e0e0; }";
    stockReportButton->setStyleSheet(buttonStyle);
    movementReportButton->setStyleSheet(buttonStyle);
    exportJsonButton->setStyleSheet(buttonStyle);
    exportPdfButton->setStyleSheet(buttonStyle);

    buttonLayout->addWidget(stockReportButton);
    buttonLayout->addWidget(movementReportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(exportJsonButton);
    buttonLayout->addWidget(exportPdfButton);

    mainLayout->addLayout(buttonLayout);

    // Период для отчётов
    QHBoxLayout *periodLayout = new QHBoxLayout();
    periodLayout->addWidget(new QLabel("Период:"));
    startDateEdit = new QDateEdit(QDate::currentDate().addDays(-30), this);
    endDateEdit = new QDateEdit(QDate::currentDate(), this);
    periodLayout->addWidget(startDateEdit);
    periodLayout->addWidget(new QLabel("—"));
    periodLayout->addWidget(endDateEdit);
    periodLayout->addStretch();
    mainLayout->addLayout(periodLayout);

    // Текст отчёта
    reportText = new QTextEdit(this);
    reportText->setReadOnly(true);
    mainLayout->addWidget(reportText);

    connect(stockReportButton, &QPushButton::clicked, this, &ReportWidget::onGenerateStockReport);
    connect(movementReportButton, &QPushButton::clicked, this, &ReportWidget::onGenerateMovementReport);
    connect(exportJsonButton, &QPushButton::clicked, this, &ReportWidget::onExportToJson);
    connect(exportPdfButton, &QPushButton::clicked, this, &ReportWidget::onExportToPdf);
}

void ReportWidget::onGenerateStockReport()
{
    // Сброс стиля всех кнопок
    stockReportButton->setStyleSheet("QPushButton { padding: 8px; border-radius: 5px; }"
                                     "QPushButton:hover { background-color: #e0e0e0; }");
    movementReportButton->setStyleSheet("QPushButton { padding: 8px; border-radius: 5px; }"
                                        "QPushButton:hover { background-color: #e0e0e0; }");

    // Подсветка нажатой кнопки
    stockReportButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; border-radius: 5px; }");

    generateStockReport();
}

void ReportWidget::generateStockReport()
{
    Database *db = Database::getInstance();
    auto stocks = db->getAllStocks();

    QString report = "=== ОТЧЁТ ПО ОСТАТКАМ ===\n\n";
    report += QString("%1 | %2 | %3\n").arg("ID", 5).arg("Товар", -30).arg("Количество", 10);
    report += QString::fromStdString(std::string(50, '-')) + "\n";

    for (const auto &stock : stocks)
    {
        Product *product = db->getProductById(stock.getProductId());
        if (product)
        {
            report += QString("%1 | %2 | %3\n")
                          .arg(product->getId(), 5)
                          .arg(QString::fromStdString(product->getName()), -30)
                          .arg(stock.getQuantity(), 10);
        }
    }

    report += QString::fromStdString(std::string(50, '-')) + "\n";
    reportText->setText(report);
}

void ReportWidget::onGenerateMovementReport()
{
    // Сброс стиля всех кнопок
    stockReportButton->setStyleSheet("QPushButton { padding: 8px; border-radius: 5px; }"
                                     "QPushButton:hover { background-color: #e0e0e0; }");
    movementReportButton->setStyleSheet("QPushButton { padding: 8px; border-radius: 5px; }"
                                        "QPushButton:hover { background-color: #e0e0e0; }");

    // Подсветка нажатой кнопки
    movementReportButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; border-radius: 5px; }");

    generateMovementReport();
}

void ReportWidget::generateMovementReport()
{
    Database *db = Database::getInstance();
    auto receipts = db->getAllReceipts();
    auto shipments = db->getAllShipments();

    QString report = "=== ОТЧЁТ ПО ДВИЖЕНИЮ ТОВАРОВ ===\n\n";

    report += "--- ПРИХОДЫ ---\n";
    for (const auto &receipt : receipts)
    {
        report += QString("Документ №%1 от %2, поставщик: %3\n")
                      .arg(receipt.getId())
                      .arg(QString::fromStdString(receipt.getDate()))
                      .arg(QString::fromStdString(receipt.getSupplier()));
        for (const auto &item : receipt.getItems())
        {
            Product *product = db->getProductById(item.getProductId());
            report += QString("  - %1: %2 шт.\n")
                          .arg(QString::fromStdString(product->getName()))
                          .arg(item.getQuantity());
        }
    }

    report += "\n--- РАСХОДЫ ---\n";
    for (const auto &shipment : shipments)
    {
        report += QString("Документ №%1 от %2, заказ №%3\n")
                      .arg(shipment.getId())
                      .arg(QString::fromStdString(shipment.getDate()))
                      .arg(shipment.getOrderId());
        for (const auto &item : shipment.getItems())
        {
            Product *product = db->getProductById(item.getProductId());
            report += QString("  - %1: %2 шт.\n")
                          .arg(QString::fromStdString(product->getName()))
                          .arg(item.getQuantity());
        }
    }

    reportText->setText(report);
}

void ReportWidget::onExportToJson()
{
    exportToJson();
}

void ReportWidget::exportToJson()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить JSON", "", "JSON files (*.json)");
    if (fileName.isEmpty())
        return;

    Database *db = Database::getInstance();
    auto stocks = db->getAllStocks();

    QString json = "{\n  \"report\": {\n    \"type\": \"stock_report\",\n    \"items\": [\n";

    for (size_t i = 0; i < stocks.size(); ++i)
    {
        Product *product = db->getProductById(stocks[i].getProductId());
        json += QString("      {\"id\": %1, \"name\": \"%2\", \"quantity\": %3}")
                    .arg(product->getId())
                    .arg(QString::fromStdString(product->getName()))
                    .arg(stocks[i].getQuantity());
        if (i < stocks.size() - 1)
            json += ",";
        json += "\n";
    }

    json += "    ]\n  }\n}";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(json.toUtf8());
        file.close();
        QMessageBox::information(this, "Успех", "Отчёт экспортирован в JSON!");
    }
}

void ReportWidget::onExportToPdf()
{
    exportToPdf();
}

void ReportWidget::exportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить PDF", "", "PDF files (*.pdf)");
    if (fileName.isEmpty())
        return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    reportText->document()->print(&printer);
    QMessageBox::information(this, "Успех", "Отчёт сохранён в PDF!");
}