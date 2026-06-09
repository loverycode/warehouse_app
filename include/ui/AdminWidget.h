#ifndef ADMIN_WIDGET_H
#define ADMIN_WIDGET_H

#include <QTabWidget>

class QWidget;

class AdminWidget : public QTabWidget
{
public:
    AdminWidget(QWidget *parent = nullptr);

private:
    QWidget *productWidget;
    QWidget *reportWidget;
};

#endif