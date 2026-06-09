#ifndef LOGIN_WIDGET_H
#define LOGIN_WIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class LoginWidget : public QWidget
{
public:
    LoginWidget(QWidget *parent = nullptr);
private slots:
    void onLogin();
    void onRegister(); // добавили
private:
    QLineEdit *loginEdit;
    QLineEdit *passEdit;
    QPushButton *loginButton;
    QPushButton *registerButton; // добавили
};

#endif