#ifndef REPORT_WIDGET_H
#define REPORT_WIDGET_H

#include <QWidget>

class QPushButton;
class QTextEdit;
class QDateEdit;

class ReportWidget : public QWidget
{
public:
    ReportWidget(QWidget *parent = nullptr);
    ~ReportWidget() = default;

private slots:
    void onGenerateStockReport();
    void onGenerateMovementReport();
    void onExportToJson();
    void onExportToPdf();

private:
    QPushButton *stockReportButton;
    QPushButton *movementReportButton;
    QPushButton *exportJsonButton;
    QPushButton *exportPdfButton;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QTextEdit *reportText;

    void generateStockReport();
    void generateMovementReport();
    void exportToJson();
    void exportToPdf();
};

#endif