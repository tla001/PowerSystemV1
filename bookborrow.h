#ifndef BOOKBORROW_H
#define BOOKBORROW_H

#include <QDialog>
#include <QTimer>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QDebug>
namespace Ui {
class BookBorrow;
}

class BookBorrow : public QDialog
{
    Q_OBJECT

public:
    explicit BookBorrow(QWidget *parent = 0);
    ~BookBorrow();
    void initSystem(QString name);

private:
    Ui::BookBorrow *ui;
    QTimer *timer;
    QString userName;

public slots:
    void doTimeUpdate();
    void doBorrowInfo();
    void doBackInfo();
    void doQueryInfo();

    void doSearchButton();
    void doBorrowButton();
    void doShowSearchResult(QString queryStr);
    void doShowBorrowArea();

    void doShowBackResult();
    void doShowBackAera();
    void doBackButton();

    void doShowQueryResult();

};

#endif // BOOKBORROW_H
