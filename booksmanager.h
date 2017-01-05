#ifndef BOOKSMANAGER_H
#define BOOKSMANAGER_H

#include <QDialog>
#include <QTimer>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidgetItem>

namespace Ui {
class BooksManager;
}

class BooksManager : public QDialog
{
    Q_OBJECT

public:
    explicit BooksManager(QWidget *parent = 0);
    ~BooksManager();

    void initSystem();

private:
    Ui::BooksManager *ui;
    QTimer *timer;

public slots:
    void doTimeUpdate();

    void dolistInfo();
    void doinkuInfo();
    void dooutkuInfo();
    void dorecordInfo();
    void doupdatelistInfo();

    void doUpdatelistInfo();
    void doUpdateinkuInfo();
    void doUpdateoutkuInfo();
    void doUpdaterecordInfo();

    void doAddBook();
    void doDelBook();
    void doUpdateBook();
    void doSearchBook();
    void doShowSearchResult(QString queryStr);
    void doShowInfoBrower(QString info);

    void doSetInputArea();

};

#endif // BOOKSMANAGER_H
