#include "bookborrow.h"
#include "ui_bookborrow.h"

BookBorrow::BookBorrow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookBorrow)
{
    ui->setupUi(this);

    setWindowTitle("自助借书系统");
    setWindowIcon(QIcon(":/main/resource/images/rendbook.ico"));

    doTimeUpdate();
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(doTimeUpdate()));
    timer->start(1000);

    connect(ui->borrowradioButton,SIGNAL(clicked(bool)),this,SLOT(doBorrowInfo()));
    connect(ui->backradioButton,SIGNAL(clicked(bool)),this,SLOT(doBackInfo()));
    connect(ui->queryradioButton,SIGNAL(clicked(bool)),this,SLOT(doQueryInfo()));
    connect(ui->searchEdit,SIGNAL(returnPressed()),this,SLOT(doSearchButton()));
    connect(ui->searchButton,SIGNAL(clicked(bool)),this,SLOT(doSearchButton()));
    connect(ui->searchtableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(doShowBorrowArea()));
    connect(ui->borrowButton,SIGNAL(clicked(bool)),this,SLOT(doBorrowButton()));
    connect(ui->backtableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(doShowBackAera()));
    connect(ui->backButton,SIGNAL(clicked(bool)),this,SLOT(doBackButton()));
}

BookBorrow::~BookBorrow()
{
    delete ui;
}
void BookBorrow::initSystem(QString name){
       userName=name;
       ui->userlabel->setText(userName);
       ui->stackedWidget->setCurrentIndex(0);
       ui->borrowradioButton->setChecked(true);
       ui->searchButton->setFocus();
}

void BookBorrow::doTimeUpdate(){
    ui->timelabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void BookBorrow::doBorrowInfo(){
 ui->stackedWidget->setCurrentIndex(0);
}
void BookBorrow::doBackInfo(){
 ui->stackedWidget->setCurrentIndex(1);
 doShowBackResult();
}
void BookBorrow::doQueryInfo(){
 ui->stackedWidget->setCurrentIndex(2);
 doShowQueryResult();
}

void BookBorrow::doSearchButton(){
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
void BookBorrow::doShowSearchResult(QString queryStr){
    QSqlQuery query;
    query.prepare(queryStr);
    if(query.exec()){
        int index=0;
        int rows=ui->searchtableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->searchtableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->searchtableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->searchtableWidget->setItem(index,0,idCenter);
                ui->searchtableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->searchtableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                int restnum=query.value(5).toInt();
                ui->searchtableWidget->setItem(index,3,new QTableWidgetItem(QString::number(restnum)));
                if(restnum>0)
                     ui->searchtableWidget->setItem(index,4,new QTableWidgetItem("YES"));
                else
                     ui->searchtableWidget->setItem(index,4,new QTableWidgetItem("NO"));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","未找到相关书籍");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
    }
}
void BookBorrow::doBorrowButton(){
    QString bookId;
    if(ui->searchbookidEdit->text().isEmpty()){
        QMessageBox::warning(this,"Failed","请选择要借阅的图书");
    }else{
        bookId=ui->searchbookidEdit->text();
        ui->searchbookauthorEdit->clear();
        ui->searchbookidEdit->clear();
        ui->searchbooknameEdit->clear();
        //查询是否已经借过
        QSqlQuery query;
        query.prepare("select * from recordlist where bookId=? and borrowPeople=? and backState=0");
        query.bindValue(0,bookId);
        query.bindValue(1,userName);
        query.exec();

        if(query.size()){
            QMessageBox::critical(this,"Error","该书已经借过！");
        }else{
            query.prepare("select * from booklist where bookId=? and restNum>0");
            query.bindValue(0,bookId);
            query.exec();
            if(query.size()){
                query.next();
                int borrownum=query.value(4).toInt();
                int restnum=query.value(5).toInt();
                QString bookName=query.value(1).toString();
                QString borrowTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                if(QSqlDatabase::database().transaction()){
                    bool res1,res2;
                    query.prepare("update booklist set borrowNum=?,restNum=? where bookId=?");
                    query.bindValue(0,++borrownum);
                    query.bindValue(1,--restnum);
                    query.bindValue(2,bookId);
                    res1=query.exec();
                    //qDebug()<<"insert "<<res1;

                    query.prepare("insert into recordlist(bookId,bookName,borrowPeople,borrowTime,backTime,\
                                  backState) values(?,?,?,?,?,?)");
                    query.bindValue(0,bookId);
                    query.bindValue(1,bookName);
                    query.bindValue(2,userName);
                    query.bindValue(3,borrowTime);
                    query.bindValue(4,"");
                    query.bindValue(5,0);
                    res2=query.exec();
                     //qDebug()<<query.lastError();

                    //qDebug()<<"insert "<<res2;
                    if(res1&&res2){
                        if(!QSqlDatabase::database().commit()){
                            QMessageBox::critical(this,"Error","操作失败，将要回滚");
                            if(!QSqlDatabase::database().rollback()){
                                QMessageBox::critical(this,"Error","回滚失败");
                            }
                        }else{
                            doShowBackResult();
                            //qDebug()<<"show ";
                        }
                    }else{
                        qDebug()<<query.lastError();
                        QMessageBox::critical(this,"Error","操作失败，将要回滚");
                        if(!QSqlDatabase::database().rollback()){
                            QMessageBox::critical(this,"Error","回滚失败");
                            qDebug()<<QSqlDatabase::database().lastError();
                        }
                    }

                }
            }else{
                qDebug()<<"not find";
            }
         }
    }
}
void BookBorrow::doShowBorrowArea(){
    int row=ui->searchtableWidget->currentRow();
     ui->searchbookidEdit->setText(ui->searchtableWidget->item(row,0)->text());
     ui->searchbooknameEdit->setText(ui->searchtableWidget->item(row,1)->text());
     ui->searchbookauthorEdit->setText(ui->searchtableWidget->item(row,2)->text());
     QString state=ui->searchtableWidget->item(row,4)->text();
     if(state=="NO"){
         ui->borrowButton->setEnabled(false);
     }else if(state=="YES"){
         ui->borrowButton->setEnabled(true);
     }
}
void BookBorrow::doShowBackResult(){
    QSqlQuery query;
    query.prepare("select * from recordlist where borrowPeople=? and backState=0 order by borrowTime");
    query.bindValue(0,userName);
    if(query.exec()){
        int index=0;
        int rows=ui->backtableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->backtableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->backtableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->backtableWidget->setItem(index,0,idCenter);
                ui->backtableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->backtableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                ui->backtableWidget->setItem(index,3,new QTableWidgetItem(query.value(3).toString()));
                ui->backtableWidget->setItem(index,4,new QTableWidgetItem("NO"));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","未找到您的借阅信息");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
    }
}
void BookBorrow::doShowBackAera(){
    int row=ui->backtableWidget->currentRow();
    ui->backbookidEdit->setText(ui->backtableWidget->item(row,0)->text());
    ui->backbooknameEdit->setText(ui->backtableWidget->item(row,1)->text());
}
void BookBorrow::doBackButton(){
    QString bookId;
    if(ui->backbookidEdit->text().isEmpty()){
        QMessageBox::warning(this,"Failed","请选择要还的图书");
    }else{
        bookId=ui->backbookidEdit->text();
        ui->backbookidEdit->clear();
        ui->backbooknameEdit->clear();
        //查询是否已经借过
        QSqlQuery query;
        query.prepare("select * from recordlist where bookId=? and borrowPeople=? and backState=0");
        query.bindValue(0,bookId);
        query.bindValue(1,userName);
        query.exec();

        if(!query.size()){
            QMessageBox::critical(this,"Error","没并有借过该书！");
        }else{
            query.prepare("select * from booklist where bookId=? and restNum>0");
            query.bindValue(0,bookId);
            query.exec();
            if(query.size()){
                query.next();
                int borrownum=query.value(4).toInt();
                int restnum=query.value(5).toInt();
                QString backTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                if(QSqlDatabase::database().transaction()){
                    bool res1,res2;
                    query.prepare("update booklist set borrowNum=?,restNum=? where bookId=?");
                    query.bindValue(0,--borrownum);
                    query.bindValue(1,++restnum);
                    query.bindValue(2,bookId);
                    res1=query.exec();
                    //qDebug()<<"insert "<<res1;

                    query.prepare("update recordlist set backTime=?,backState=? where bookId=?");
                    query.bindValue(0,backTime);
                    query.bindValue(1,1);
                    query.bindValue(2,bookId);
                    res2=query.exec();
                     //qDebug()<<query.lastError();

                    //qDebug()<<"insert "<<res2;
                    if(res1&&res2){
                        if(!QSqlDatabase::database().commit()){
                            QMessageBox::critical(this,"Error","操作失败，将要回滚");
                            if(!QSqlDatabase::database().rollback()){
                                QMessageBox::critical(this,"Error","回滚失败");
                            }
                        }else{
                            doShowBackResult();
                            //qDebug()<<"show ";
                        }
                    }else{
                        qDebug()<<query.lastError();
                        QMessageBox::critical(this,"Error","操作失败，将要回滚");
                        if(!QSqlDatabase::database().rollback()){
                            QMessageBox::critical(this,"Error","回滚失败");
                            qDebug()<<QSqlDatabase::database().lastError();
                        }
                    }

                }
            }else{
                qDebug()<<"not find";
            }
         }
    }
}
void BookBorrow::doShowQueryResult(){
    QSqlQuery query;
    query.prepare("select * from recordlist where borrowPeople=? order by borrowTime");
    query.bindValue(0,userName);
    if(query.exec()){
        int index=0;
        int rows=ui->querytableWidget->rowCount();
        for(int i=0;i<rows;i++){
            ui->querytableWidget->removeRow(0);
        }
        if(query.size()){
            while(query.next()){
                ui->querytableWidget->insertRow(index);
                QTableWidgetItem *idCenter = new QTableWidgetItem(query.value(0).toString());
                idCenter->setTextAlignment(Qt::AlignCenter);
                ui->querytableWidget->setItem(index,0,idCenter);
                ui->querytableWidget->setItem(index,1,new QTableWidgetItem(query.value(1).toString()));
                ui->querytableWidget->setItem(index,2,new QTableWidgetItem(query.value(2).toString()));
                ui->querytableWidget->setItem(index,3,new QTableWidgetItem(query.value(3).toString()));
                ui->querytableWidget->setItem(index,4,new QTableWidgetItem(query.value(4).toString()));
                bool backstat=query.value(5).toBool();
                if(backstat)
                    ui->querytableWidget->setItem(index,5,new QTableWidgetItem("YES"));
                else
                    ui->querytableWidget->setItem(index,5,new QTableWidgetItem("NO"));

                index++;
            }
        }else{
            QMessageBox::warning(this,"Failed","未找到您的借阅信息");
        }
    }else{
       QMessageBox::critical(this,"Error","执行失败");
    }
}
