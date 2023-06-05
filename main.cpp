
#include "mainwindow.h"
#include <QApplication>

/** \mainpage
 *  \par Micromouse
 *  Program symulujący zawody 'micromouse', polegające na pokonywaniu labiryntu
 *  przez autonomiczne roboty.
 *  \author Mateusz Frejlich
 *  \date 29.05.2023
 *  \version 1.0
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
