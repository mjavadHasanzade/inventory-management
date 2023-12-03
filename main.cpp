#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Load the stylesheet file
    QFile styleFile(":css/style.css"); // Replace with the actual path to your stylesheet file

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = QString::fromUtf8(styleFile.readAll());
        qDebug()<<styleSheet.length();
        a.setStyleSheet(styleSheet);
    }

    w.show();

    return a.exec();
}
