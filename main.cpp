//   ______                _ ______
//  (_____ \              | (_____ \
//   _____) )__  _ _ _  __| |_____) ) ____
//  |  ____/ _ \| | | |/ _  (_____ ( / ___)
//  | |   | |_| | | | ( (_| |_____) ) |
//  |_|    \___/ \___/ \____(______/|_|



#include "src/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

