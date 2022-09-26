#include "trafanalis.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrafAnalis w;
    w.show();

    return a.exec();
}
