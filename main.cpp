#include "inc/filemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileManager w;
    w.show();
    return a.exec();
}
