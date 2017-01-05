#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
    int reIndex();

private slots:
    void on_pushButton_clicked();
    void registButtonClicked();


private:
    Ui::login *ui;
    QString userName;
    QString passwd;
    QSqlTableModel *model;
    int index;
};

#endif // LOGIN_H
