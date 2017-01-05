#include "booksmanager.h"
#include "ui_booksmanager.h"
#include <QtSql>
#include <QSqlQuery>
#include <QDebug>

BooksManager::BooksManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BooksManager)
{
    ui->setupUi(this);
    setWindowTitle("书库管理系统");
    setWindowIcon(QIcon(":/main/resource/images/books.ico"));

    doTimeUpdate();
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(doTimeUpdate()));
    timer->start(1000);

    connect(ui->alllistButton,SIGNAL(clicked(bool)),this,SLOT(dolistInfo()));
    connect(ui->inkuButton,SIGNAL(clicked(bool)),this,SLOT(doinkuInfo()));
    connect(ui->outkuButton,SIGNAL(clicked(bool)),this,SLOT(dooutkuInfo()));
    connect(ui->recordButton,SIGNAL(clicked(bool)),this,SLOT(dorecordInfo()));
    connect(ui->updatelistButton,SIGNAL(clicked(bool)),this,SLOT(doupdatelistInfo()));

    connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(doAddBook()));
    connect(ui->delButton,SIGNAL(clicked(bool)),this,SLOT(doDelBook()));
    connect(ui->updateButton,SIGNAL(clicked(bool)),this,SLOT(doUpdateBook()));
    connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(doSearchBook()));

    connect(ui->alllisttableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(doSetInputArea()));

}

BooksManager::~BooksManager()
{
    delete ui;
}

void BooksManager::initSystem(){
    QSqlQuery query;
    bool res;
    res=query.exec("CREATE TABLE  if not EXISTS booklist(\
               bookId VARCHAR(20) PRIMARY KEY,\
               bookName VARCHAR(20) NOT NULL,\
               authorName VARCHAR(20),\
                   totalNum INT NOT NULL,\
                   borrowNum INT NOT NULL,\
                   restNum INT NOT NULL)");
    //qDebug()<<res;
    res=query.exec("CREATE TABLE  if not EXISTS recordlist(\
               bookId VARCHAR(20) not null,\
               bookName VARCHAR(20) NOT NULL,\
               borrowPeople VARCHAR(20) NOT NULL,\
                   borrowTime VARCHAR(30) NOT NULL,\
                   backTime VARCHAR(30) NOT NULL,\
                   backState TINYINT NOT NULL)");
    //qDebug()<<res;
    ui->stackedWidget->setCurrentIndex(0);
    doUpdatelistInfo();
    doUpdateinkuInfo();
    doUpdateoutkuInfo();
    doUpdaterecordInfo();
}
void BooksManager::doTimeUpdate(){
    ui->timelabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}
void BooksManager::dolistInfo(){
    ui->stackedWidget->setCurrentIndex(0);
}
void BooksManager::doinkuInfo(){
    ui->stackedWidget->setCurrentIndex(1);

}
void BooksManager::dooutkuInfo(){
    ui->stackedWidget->setCurrentIndex(2);
}
void BooksManager::dorecordInfo(){
    ui->stackedWidget->setCurrentIndex(3);
}
void BooksManager::doupdatelistInfo(){
    int index=ui->stackedWidget->currentIndex();
    switch(index){
    case 0:
        doUpdatelistInfo();
        break;
    case 1:
        doUpdateinkuInfo();
        break;
    case 2:
        doUpdateoutkuInfo();
        break;
    case 3:
        doUpdaterecordInfo();
        break;
    }
}
void BooksManager::doUpdatelistInfo(){
    QSqlQuery query;
    query.prepare("select * from booklist order by bookId");//
    query.exec();
    int index=0;
    int rows=ui->alllisttableWidget->rowCount();
    for(int i=0;i<rows;i++){
        ui->alllisttableWidget->removeRow(0);
       // qDebug()<<"rows "<<ui->alllisttableWidget->rowCount();
    }
    //qDebug()<<endl;
    while(query.next()){
       // qDebug()<<"index "<<index;
        ui->alllisttableWidget->insertRow(index);
        QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
        idCenter->setTextAlignment(Qt::AlignCenter);
        ui->alllisttableWidget->setItem(index,0,idCenter);
        ui->alllisttableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
        ui->alllisttableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
        ui->alllisttableWidget->setItem(index,3,new QTableWidgetItem(QString::number(query.value(3).toInt())));
        ui->alllisttableWidget->setItem(index,4,new QTableWidgetItem(QString::number(query.value(4).toInt())));
        ui->alllisttableWidget->setItem(index,5,new QTableWidgetItem(QString::number(query.value(5).toInt())));
//        for(int i=0;i<6;i++)
//        qDebug()<<query.value(i).toString()<<"\t";
        index++;
    }
    //qDebug()<<ui->alllisttableWidget->rowCount();
}
void BooksManager::doUpdateinkuInfo(){
    QSqlQuery query;
    query.prepare("select * from booklist where restNum>0 order by bookId");//
    query.exec();
    int index=0;
    int rows=ui->inkutableWidget->rowCount();
    for(int i=0;i<rows;i++){
        ui->inkutableWidget->removeRow(0);
       // qDebug()<<"rows "<<ui->alllisttableWidget->rowCount();
    }
    //qDebug()<<endl;
    while(query.next()){
       // qDebug()<<"index "<<index;
        ui->inkutableWidget->insertRow(index);
        QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
        idCenter->setTextAlignment(Qt::AlignCenter);
        ui->inkutableWidget->setItem(index,0,idCenter);
        ui->inkutableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
        ui->inkutableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
        ui->inkutableWidget->setItem(index,3,new QTableWidgetItem(QString::number(query.value(5).toInt())));
        ui->inkutableWidget->setItem(index,4,new QTableWidgetItem("YES"));

        index++;
    }
}
void BooksManager::doUpdateoutkuInfo(){
    QSqlQuery query;
    query.prepare("select * from recordlist where backState=0 order by borrowTime");
    if(query.exec()){
        int index=0;
        int rows=ui->outkutableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->outkutableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->outkutableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->outkutableWidget->setItem(index,0,idCenter);
                ui->outkutableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->outkutableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                ui->outkutableWidget->setItem(index,3,new QTableWidgetItem(query.value(3).toString()));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","当前没有借出信息");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
       qDebug()<<query.lastError();
    }
}
void BooksManager::doUpdaterecordInfo(){
    QSqlQuery query;
    query.prepare("select * from recordlist  order by borrowTime");
    if(query.exec()){
        int index=0;
        int rows=ui->recordtableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->recordtableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->recordtableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->recordtableWidget->setItem(index,0,idCenter);
                ui->recordtableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->recordtableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                ui->recordtableWidget->setItem(index,3,new QTableWidgetItem(query.value(3).toString()));
                ui->recordtableWidget->setItem(index,4,new QTableWidgetItem(query.value(4).toString()));
                bool backstat=query.value(5).toBool();
                if(backstat)
                    ui->recordtableWidget->setItem(index,5,new QTableWidgetItem("YES"));
                else
                    ui->recordtableWidget->setItem(index,5,new QTableWidgetItem("NO"));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","当前没有借出信息");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
    }
}


void BooksManager::doAddBook(){
    QString bookId,bookName,authorName;
    int totalNum;
    if(ui->bookidEdit->text().isEmpty()||ui->booknameEdit->text().isEmpty()
            ||ui->bookauthorEdit->text().isEmpty()||ui->booktotalEdit->text().isEmpty()){
        QMessageBox::warning(this,"Error","添加新书时，信息不能为空");
        ui->bookidEdit->setFocus();
    }else{
        QSqlQuery query;
        query.prepare("select * from booklist where bookId=?");
        query.bindValue(0,ui->bookidEdit->text());
        query.exec();
//        bool res=
//        qDebug()<<res;
        if(query.size()){
            QMessageBox::critical(this,"Error","数目编号已存在！");
            ui->bookidEdit->clear();
            ui->bookidEdit->setFocus();
        }else{
            bookId=ui->bookidEdit->text();
            bookName=ui->booknameEdit->text();
            authorName=ui->bookauthorEdit->text();
            totalNum=ui->booktotalEdit->text().toInt();
            query.prepare("insert into booklist(bookId,bookName,authorName,totalNum,\
                          borrowNum,restNum) values(?,?,?,?,?,?)");
            query.bindValue(0,bookId);
            query.bindValue(1,bookName);
            query.bindValue(2,authorName);
            query.bindValue(3,totalNum);
            query.bindValue(4,0);
            query.bindValue(5,totalNum);
            if(query.exec()){
                QMessageBox::information(this,"Success","添加成功",QMessageBox::Ok);
                doUpdatelistInfo();
            }else{
                QMessageBox::critical(this,"Error","添加失败",QMessageBox::Cancel);
                qDebug()<<query.lastError();
            }
        }
    }
}
void BooksManager::doDelBook(){
    QSqlQuery query;
    if(ui->bookidEdit->text().isEmpty()){
       QTableWidgetItem *item= ui->alllisttableWidget->item(ui->alllisttableWidget->currentRow(),0);
        //qDebug()<<item->text();
       query.prepare("select * from booklist where bookId=?");
       query.bindValue(0,item->text());
       query.exec();
       query.next();
      int restnum= query.value(5).toInt();
      if(restnum>0){
          QMessageBox::critical(this,"Error","存在借出书，暂不能删除");
      }else{
          query.prepare("DELETE FROM booklist WHERE bookId=?");
          query.bindValue(0,item->text());
          query.exec();
          if(query.size()){
              QMessageBox::information(this,"Success","删除成功");
              ui->bookidEdit->clear();
              doUpdatelistInfo();
          }else{
              QMessageBox::warning(this,"Failed","删除失败");
          }
      }

    }else{
        query.prepare("select * from booklist where bookId=?");
        query.bindValue(0,ui->bookidEdit->text());
        query.exec();
        if(query.size()){
            query.next();
           int restnum= query.value(5).toInt();
           if(restnum>0){
               QMessageBox::critical(this,"Error","存在借出书，暂不能删除");
           }else{
               query.prepare("DELETE FROM booklist WHERE bookId=?");
               query.bindValue(0,ui->bookidEdit->text());
               query.exec();
               if(query.size()){
                   QMessageBox::information(this,"Success","删除成功");
                   ui->bookidEdit->clear();
                   doUpdatelistInfo();
               }else{
                   QMessageBox::warning(this,"Failed","删除失败");
               }
           }
        }else{
            QMessageBox::critical(this,"Error","没有找到，请重新输入id");
            ui->bookidEdit->clear();
            ui->bookidEdit->setFocus();
        }
    }
}
void BooksManager::doUpdateBook(){
    QString bookId,bookName,authorName;
    int totalNum;
    if(ui->bookidEdit->text().isEmpty()||ui->booknameEdit->text().isEmpty()
            ||ui->bookauthorEdit->text().isEmpty()||ui->booktotalEdit->text().isEmpty()){
        QMessageBox::warning(this,"Error","更新书籍信息时，信息不能为空");
        ui->bookidEdit->setFocus();
    }else{
        QSqlQuery query;
        query.prepare("select * from booklist where bookId=?");
        query.bindValue(0,ui->bookidEdit->text());
        query.exec();
//        bool res=
//        qDebug()<<res;
        if(query.size()==0){
            QMessageBox::critical(this,"Error","数目编号不存在！");
            ui->bookidEdit->clear();
            ui->bookidEdit->setFocus();
        }else{
            int orestNum=0;
            int ototalNum=0;
            if(query.next()){
                orestNum=query.value(5).toInt();
                ototalNum=query.value(3).toInt();
               // qDebug()<<orestNum<<"\t"<<ototalNum;
            }
            bookId=ui->bookidEdit->text();
            bookName=ui->booknameEdit->text();
            authorName=ui->bookauthorEdit->text();
            totalNum=ui->booktotalEdit->text().toInt();
            query.prepare("update booklist set bookName=?,authorName=?,totalNum=?,\
                          restNum=? where bookId=?");
            query.bindValue(0,bookName);
            query.bindValue(1,authorName);
            query.bindValue(2,totalNum);
            query.bindValue(3,orestNum+totalNum-ototalNum);
            query.bindValue(4,bookId);
            if(query.exec()){
                QMessageBox::information(this,"Success","更新成功",QMessageBox::Ok);
                doUpdatelistInfo();
            }else{
                QMessageBox::critical(this,"Error","更新失败",QMessageBox::Cancel);
                qDebug()<<query.lastError();
            }
        }
    }
}
void BooksManager::doSearchBook(){
    QString label=ui->indexBox->currentText();
    QString key;
    QString queryStr="select * from booklist where ";
    //qDebug()<<label;
    if(label=="书名"){
        if(ui->searchEdit->text().isEmpty()){
            QMessageBox::warning(this,"Failed","请输入书名关键字",QMessageBox::Cancel);
        }else{
            key=ui->searchEdit->text();
            queryStr+="bookName='"+key+"'";
            doShowSearchResult(queryStr);
        }
    }else if(label=="书号"){
        if(ui->searchEdit->text().isEmpty()){
            QMessageBox::warning(this,"Failed","请输入书号关键字",QMessageBox::Cancel);
        }else{
            key=ui->searchEdit->text();
            queryStr+="bookId='"+key+"'";
            doShowSearchResult(queryStr);
        }
    }else if(label=="作者"){
        if(ui->searchEdit->text().isEmpty()){
            QMessageBox::warning(this,"Failed","请输入作者关键字",QMessageBox::Cancel);
        }else{
            key=ui->searchEdit->text();
            queryStr+="authorName='"+key+"'";
            doShowSearchResult(queryStr);
        }
    }
}
void BooksManager::doShowSearchResult(QString queryStr){
   // qDebug()<<queryStr;
    QSqlQuery query;
    query.prepare(queryStr);
    if(query.exec()){
        int index=0;
        int rows=ui->resulttableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->resulttableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->resulttableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->resulttableWidget->setItem(index,0,idCenter);
                ui->resulttableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->resulttableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                int restnum=query.value(5).toInt();
                ui->resulttableWidget->setItem(index,3,new QTableWidgetItem(QString::number(restnum)));
                if(restnum)
                     ui->resulttableWidget->setItem(index,4,new QTableWidgetItem("YES"));
                else
                     ui->resulttableWidget->setItem(index,4,new QTableWidgetItem("NO"));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","未找到相关书籍");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
    }
 }
void BooksManager::doShowInfoBrower(QString info){

}
void BooksManager::doSetInputArea(){
    int row=ui->alllisttableWidget->currentRow();
     ui->bookidEdit->setText(ui->alllisttableWidget->item(row,0)->text());
     ui->booknameEdit->setText(ui->alllisttableWidget->item(row,1)->text());
     ui->bookauthorEdit->setText(ui->alllisttableWidget->item(row,2)->text());
     ui->booktotalEdit->setText(ui->alllisttableWidget->item(row,3)->text());
}
