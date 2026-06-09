#include "ui/LoginWidget.h"
#include "ui/EmployeeWidget.h"
#include "ui/AdminWidget.h"
#include "database/Database.h"
#include "core/User.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Авторизация", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    loginEdit = new QLineEdit(this);
    loginEdit->setPlaceholderText("Логин");

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Войти", this);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(loginEdit);
    mainLayout->addWidget(passEdit);
    mainLayout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::onLogin);
}

void LoginWidget::onLogin()
{
    QString login = loginEdit->text();
    QString pass = passEdit->text();

    qDebug() << "=== ПОПЫТКА ВХОДА ===";
    qDebug() << "Логин:" << login;
    qDebug() << "Пароль:" << pass;

    Database *db = Database::getInstance();
    if (!db->connect())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных!");
        return;
    }

    User *user = db->getUserByLogin(login.toStdString());

    if (!user)
    {
        qDebug() << "❌ Пользователь не найден в БД";
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
        return;
    }

    qDebug() << "✅ Пользователь найден: ID=" << user->getId()
             << ", роль=" << (user->getRole() == Role::Admin ? "admin" : "employee");

    bool passwordOk = user->login(login.toStdString(), pass.toStdString());
    qDebug() << "Проверка пароля:" << (passwordOk ? "✅ верный" : "❌ неверный");

    if (!passwordOk)
    {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
        return;
    }

    qDebug() << "✅ Пароль верный, открываем окно...";

    if (user->getRole() == Role::Admin)
    {
        qDebug() << "→ Открываем окно АДМИНИСТРАТОРА";
        AdminWidget *adminWidget = new AdminWidget();
        adminWidget->show();
    }
    else
    {
        qDebug() << "→ Открываем окно СОТРУДНИКА";
        EmployeeWidget *employeeWidget = new EmployeeWidget();
        employeeWidget->show();
    }

    this->close();
}