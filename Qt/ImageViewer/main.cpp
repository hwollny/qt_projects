#include "imageviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w;
   //w.setAttribute(Qt::WA_DeleteOnClose); //close and delete all widgets when main window is closed
    w.show();
    a.exec();
    a.exit();
    return 0;//
}
