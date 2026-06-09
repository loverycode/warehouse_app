#include "ui/RegisterWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

RegisterWidget::RegisterWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Регистрация", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    loginEdit = new QLineEdit(this);
    loginEdit->setPlaceholderText("Логин");

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Пароль");
    passEdit->setEchoMode(QLineEdit::Password);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Имя");

    roleCombo = new QComboBox(this);
    roleCombo->addItem("Сотрудник склада");
    roleCombo->addItem("Администратор");

    registerButton = new QPushButton("Зарегистрироваться", this);
    backButton = new QPushButton("Назад", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(registerButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(loginEdit);
    mainLayout->addWidget(passEdit);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(roleCombo);
    mainLayout->addLayout(buttonLayout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegister);
    connect(backButton, &QPushButton::clicked, this, &RegisterWidget::onBackToLogin);
}

void RegisterWidget::onRegister()
{
    QString login = loginEdit->text();
    QString pass = passEdit->text();
    QString name = nameEdit->text();
    int roleIndex = roleCombo->currentIndex();

    if (login.isEmpty() || pass.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    // TODO: сохранить пользователя в базу данных
    QMessageBox::information(this, "Успех", "Регистрация успешна!\nТеперь войдите в систему.");

    onBackToLogin();
}

void RegisterWidget::onBackToLogin()
{
    // Возврат к окну авторизации
    if (parentWidget())
    {
        parentWidget()->show();
    }
    this->close();
}