#include "mainwindow.h"
#include <QApplication>


#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;

    format.setVersion(4, 0); // requested OpenGL version; you might have to change this
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions);
    format.setOption(QSurfaceFormat::DebugContext);

    QSurfaceFormat::setDefaultFormat(format);
    MainWindow w;
    w.show();

    return a.exec();
}
