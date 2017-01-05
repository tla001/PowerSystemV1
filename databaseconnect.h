#ifndef DATABASECONNECT_H
#define DATABASECONNECT_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

static bool createConnection(){
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("powersystem");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("root");
    if(!db.open()){
        QMessageBox::critical(0,"Error",
                              "Unable to establish a database connection.",
                              QMessageBox::Cancel);
        return false;
    }
    return true;
}


#endif // DATABASECONNECT_H
