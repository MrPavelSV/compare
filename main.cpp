#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr( QTextCodec::codecForName("CP1251") );
    Widget w;
    w.show();
    return a.exec();
}
