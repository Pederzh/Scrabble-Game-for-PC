#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //dialog chiedi num giocatori

    MainWindow w;
    w.show();


    return a.exec();
}
