/****************************************************************************
 * Copyright ©2017 Brian Curless.  All rights reserved.  Permission is hereby
 * granted to students registered for University of Washington CSE 457 or CSE
 * 557 for use solely during Autumn Quarter 2017 for purposes of the course.
 * No other use, copying, distribution, or modification is permitted without
 * prior written consent. Copyrights for third-party components of this work
 * must be honored.  Instructors interested in reusing these course materials
 * should contact the author.
 ****************************************************************************/

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    // Represents the format of a renderable surface
    // NOTE: If not set as default, Mac renders as black screen
    QSurfaceFormat glFormat;
    glFormat.setRenderableType( QSurfaceFormat::OpenGL );
    glFormat.setMajorVersion( 4 );
    glFormat.setMinorVersion( 1 );
    glFormat.setProfile( QSurfaceFormat::CoreProfile ); // Functionality deprecated in OpenGL 3.0 is not available.
    glFormat.setSwapBehavior( QSurfaceFormat::DoubleBuffer );
//    glFormat.setStencilBufferSize( 8 );
//    glFormat.setSamples(4);
    QSurfaceFormat::setDefaultFormat(glFormat);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    // High DPI Support
//    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
