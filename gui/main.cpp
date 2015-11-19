#include <iostream>
#include <QApplication>
#include <QGLFormat>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        QDir::setCurrent(QCoreApplication::applicationDirPath());

        QGLFormat glf = QGLFormat::defaultFormat();
        glf.setSampleBuffers(true);
        glf.setSamples(4);
        QGLFormat::setDefaultFormat(glf);

        MainWindow w;
        w.show();
    
        return a.exec();
    } catch (std::string error) {
        std::cerr << "Error: " << error << std::endl;
    }
}
