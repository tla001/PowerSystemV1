#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>

namespace Ui {
class UserManager;
}

class UserManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = 0);
    ~UserManager();

private:
    Ui::UserManager *ui;
};

#endif // USERMANAGER_H
