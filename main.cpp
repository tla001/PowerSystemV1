#include "mainsystem.h"
#include "login.h"
#include <QApplication>
#include "databaseconnect.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createConnection())
        return -1;
    login l;
   if(!l.exec()) return -1;
    MainSystem w;
    w.show();
    w.initSystem(l.reIndex());
    return a.exec();
}
