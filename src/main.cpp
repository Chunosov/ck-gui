#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ck-gui");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
