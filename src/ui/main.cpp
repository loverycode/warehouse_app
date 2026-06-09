#include <QApplication>
#include "ui/LoginWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LoginWidget loginWidget;
    loginWidget.show();

    return app.exec();
}