#ifndef MAINSYSTEM_H
#define MAINSYSTEM_H

#include <QMainWindow>
#include <QListWidget>
#include <QtSql>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include "booksmanager.h"

namespace Ui {
class MainSystem;
}

class MainSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainSystem(QWidget *parent = 0);
    ~MainSystem();
    void initSystem(int index);
    int reIndex(){
        return index;
    }

private:
    Ui::MainSystem *ui;
    QSqlTableModel *model;
    QString username;
    bool admin;
    int index;
    QTimer *timer;

public slots:
    void doLogOut();
    void doTimeUpdate();
    void doBooksManager();
    void doBookBorrow();
};

#endif // MAINSYSTEM_H
