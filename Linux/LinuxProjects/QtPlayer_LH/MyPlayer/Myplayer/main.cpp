#include "widget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.setWindowTitle("视频");
    //w.setWindowIcon(QIcon(":/new/prefix1/icon.ico"));
    //w.setAttribute(Qt::WA_TranslucentBackground, false);


    w.show();

    return a.exec();
}
