#include "mainwindow.h"
#include <QApplication>

extern int  DISPLAY_CX;
extern int  DISPLAY_CY;


int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    ////open appl from another path keeping the application's path
    ////argv[0] = C:\ArtCash\ArtCash\KeyPainter.exe
    //if(argc>1)
    {
        QFileInfo inf(argv[0]);
        QDir::setCurrent(inf.path());
        //QProcess::setWorkingDirectory(inf.path());
    }
    /////////////////////////////
    MainWindow w;
    //w.resize(350,DISPLAY_CY);
    w.setGeometry(DISPLAY_CX-320, 30, 320, DISPLAY_CY-30);
    w.setWindowTitle("KeyPainter");
    w.show();

    /////////////////////////////
    ////direct open kpd from exe arguments
    ////argv[0] = C:\ArtCash\ArtCash\KeyPainter.exe
    if(argc>1) w.OpenFile(argv[1]);

    return a.exec();
}
