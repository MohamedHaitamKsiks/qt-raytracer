#include "mainwindow.h"

#include <QApplication>
#include "random.h"

int main(int argc, char *argv[])
{
    Random::init();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
