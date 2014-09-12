#include <iostream>
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    try {
        return a.exec();
    } catch (std::string error) {
        std::cerr << "Error: " << error << std::endl;
    }
}
