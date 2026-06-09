#include "ui/MainWindow.h"
#include "ui/LoginWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setCentralWidget(new LoginWidget(this));
    setWindowTitle("Система управления складом");
    resize(800, 600);
}