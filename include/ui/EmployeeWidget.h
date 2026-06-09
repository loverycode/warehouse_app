#ifndef EMPLOYEE_WIDGET_H
#define EMPLOYEE_WIDGET_H

#include <QTabWidget>

class EmployeeWidget : public QTabWidget
{
public:
    EmployeeWidget(QWidget *parent = nullptr);
    ~EmployeeWidget(); // ← добавить объявление деструктора
};

#endif