#ifndef REGISTER_WIDGET_H
#define REGISTER_WIDGET_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QPushButton;

class RegisterWidget : public QWidget
{
public:
    RegisterWidget(QWidget *parent = nullptr);
private slots:
    void onRegister();
    void onBackToLogin();

private:
    QLineEdit *loginEdit;
    QLineEdit *passEdit;
    QLineEdit *nameEdit;
    QComboBox *roleCombo;
    QPushButton *registerButton;
    QPushButton *backButton;
};

#endif