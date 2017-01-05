#include "mainsystem.h"
#include "ui_mainsystem.h"
#include "login.h"
#include "bookborrow.h"


MainSystem::MainSystem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainSystem)
{
    ui->setupUi(this);
    ui->menuBar->setStyleSheet("font: 10pt");
    setWindowTitle("PowerfulSystem--主界面");
    setWindowIcon(QIcon(":/main/resource/images/main.ico"));

    QListWidget *list = new QListWidget;
    list->insertItem(0,"信息管理");
    list->insertItem(1,"其他软件");
    list->insertItem(2,"功能展示");
    list->setStyleSheet("font: 10pt");
    ui->tabWidget->addTab(list,"服务选择");
    connect(list,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
    connect(ui->quitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->logoutAction,SIGNAL(triggered(bool)),this,SLOT(doLogOut()));

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(doTimeUpdate()));
    timer->start(1000);

    connect(ui->booksManagerButton,SIGNAL(clicked(bool)),this,SLOT(doBooksManager()));
    connect(ui->bookBorrowButton,SIGNAL(clicked(bool)),this,SLOT(doBookBorrow()));
}

MainSystem::~MainSystem()
{
    delete ui;
}
void MainSystem::initSystem(int index){
    this->index=index;
    model = new QSqlTableModel;
    model->setTable("userlist");
    model->select();

    QSqlRecord record = model->record(this->index);
    username = record.value("username").toString();
//    pswd = record.value("UerPaswd").toString();
//    powr = record.value("UserPower").toString();
    admin = record.value("admin").toBool();
    ui->userlabel->setText(username);
    if(admin)
         ui->adminlabel->setText("是");
    else
        ui->adminlabel->setText("否");
    doTimeUpdate();
}
void MainSystem::doLogOut(){
   this->close();
    login l;
   if(!l.exec()) return;
   this->show();
   this->initSystem(l.reIndex());
}
void MainSystem::doTimeUpdate(){
    ui->timelabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->datelabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
}
void MainSystem::doBooksManager(){
    if(admin){
        BooksManager bm;
        this->hide();
        bm.initSystem();
        bm.exec();
        this->show();
    }else{
        QMessageBox::warning(this,"Failed","当前用户没有权限！");
    }
}

void MainSystem::doBookBorrow(){
    BookBorrow bb;
    this->hide();
    bb.initSystem(username);
    bb.exec();
    this->show();
}
