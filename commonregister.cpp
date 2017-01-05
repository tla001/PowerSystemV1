#include "commonregister.h"
#include "ui_commonregister.h"
#include <QDebug>

CommonRegister::CommonRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonRegister)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/register/resource/images/register.ico"));
    setWindowTitle("普通用户注册");


    connect(ui->submitButton,SIGNAL(clicked(bool)),this,SLOT(doSubmit()));
    connect(ui->resetButton,SIGNAL(clicked(bool)),this,SLOT(doReset()));
    connect(ui->quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

CommonRegister::~CommonRegister()
{
    delete ui;
}

void CommonRegister::doSubmit(){
    //查重
    QSqlQuery query,query1;
    query.exec("create table if not EXISTS userlist(\
               id INT PRIMARY KEY AUTO_INCREMENT,\
               username VARCHAR(20) NOT NULL,\
               usergender VARCHAR(2) NOT NULL,\
               userpasswd VARCHAR(20) NOT NULL,\
               admin TINYINT NOT NULL,\
               userphone VARCHAR(11),\
               userqq VARCHAR(20),\
               useremail VARCHAR(30) )");
    query.prepare("select * from userlist where username=?");
    query.addBindValue(ui->nameEdit->text());
    query.exec();
    if(query.size()>0){
        QMessageBox::warning(this,"Failed","用户已存在，请重新输入！");
        ui->nameEdit->clear();
        ui->nameEdit->setFocus();
    }else if(ui->nameEdit->text().isEmpty()){
        QMessageBox::warning(this,"Error","用户名不能为空！");
        ui->nameEdit->setFocus();
    }else if(ui->nameEdit->text().size()>20){
        QMessageBox::warning(this,"Error","用户名不能超过20个字符！");
        ui->nameEdit->setFocus();
    }else if(ui->passwdEdit->text().isEmpty()){
        QMessageBox::warning(this,"Wrong","密码不能为空！");
        ui->repasswdEdit->clear();
        ui->passwdEdit->setFocus();
    }else if(ui->passwdEdit->text().size()>20){
        QMessageBox::warning(this,"Wrong","密码不能超过20个字符！");
        ui->repasswdEdit->clear();
        ui->passwdEdit->setFocus();
    }else if(ui->passwdEdit->text()!=ui->repasswdEdit->text()){
        QMessageBox::warning(this,"Wrong","密码不一致，请重新输入！");
        ui->passwdEdit->clear();
        ui->repasswdEdit->clear();
        ui->passwdEdit->setFocus();
    }else if(ui->phoneEdit->text().size()>11){
        QMessageBox::warning(this,"Wrong","手机号不能超过11个字符！");
        ui->phoneEdit->setFocus();
    }else if(ui->qqEdit->text().size()>20){
        QMessageBox::warning(this,"Wrong","qq号不能超过20个字符！");
        ui->qqEdit->setFocus();
    }else if(ui->emailEdit->text().size()>20){
        QMessageBox::warning(this,"Wrong","eamil不能超过30个字符！");
        ui->emailEdit->setFocus();
        //可以正则进行合法性检查
    }else{
        userName=ui->nameEdit->text();
        gender=ui->genderBox->currentText();
        passwd=ui->passwdEdit->text();
        admin=0;
        phone=ui->phoneEdit->text();
        qq=ui->qqEdit->text();
        email=ui->emailEdit->text();

        query1.prepare("insert into userlist(username,usergender,userpasswd,\
                       admin,userphone,userqq,useremail) \
                        values(?,?,?,?,?,?,?)");
        query1.bindValue(0,userName);
        query1.bindValue(1,gender);
        query1.bindValue(2,passwd);
        query1.bindValue(3,admin);
        query1.bindValue(4,phone);
        query1.bindValue(5,qq);
        query1.bindValue(6,email);

        if(!query1.exec()){
            QMessageBox::warning(this,"Failed","注册失败！");
        }else{

            QMessageBox::information(this,"Success","注册成功");
            this->close();
        }
    }
}

void CommonRegister::doReset(){
    ui->nameEdit->clear();
    ui->passwdEdit->clear();
    ui->emailEdit->clear();
    ui->repasswdEdit->clear();
    ui->qqEdit->clear();
    ui->phoneEdit->clear();
//    gender=ui->genderBox->currentText();
//    qDebug()<<gender;
}
