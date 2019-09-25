#include <QApplication>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(textures);

    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;
    window.show();
    return app.exec();
}
