#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QFont f;
    f.setPixelSize(11);
    a.setFont(f);

    MainWindow w;
    w.show();
    return a.exec();
}
