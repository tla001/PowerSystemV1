#ifndef COMMONREGISTER_H
#define COMMONREGISTER_H

#include <QDialog>
#include <QString>
#include <QSqlTableModel>
#include <QtSql>
#include <QMessageBox>
#include <QSqlQuery>

namespace Ui {
class CommonRegister;
}

class CommonRegister : public QDialog
{
    Q_OBJECT

public:
    explicit CommonRegister(QWidget *parent = 0);
    ~CommonRegister();

private:
    Ui::CommonRegister *ui;
    QString userName;
    QString passwd;
    QString gender;
    char admin;
    QString phone;
    QString qq;
    QString email;

public slots:
    void doSubmit();
    void doReset();
};

#endif // COMMONREGISTER_H
