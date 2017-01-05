#include "login.h"
#include "ui_login.h"
#include "commonregister.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/new/login/resource/images/login.ico"));
    setWindowTitle("用户登录");
    ui->line_UserPasswd->setEchoMode(QLineEdit::Password);

    model=new QSqlTableModel();
    model->setTable("userlist");
    model->select();

    connect(ui->registButton,SIGNAL(clicked(bool)),this,SLOT(registButtonClicked()));
    connect(ui->quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    ui->line_UserName->setText("admin");
    ui->line_UserPasswd->setText("admin");
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    if(ui->line_UserName->text().isEmpty()){
        QMessageBox::warning(this,"登录失败","请输入账号！",QMessageBox::Ok);
        ui->line_UserName->setFocus();
    }else if(ui->line_UserPasswd->text().isEmpty()){
        QMessageBox::warning(this,"登录失败","请输入密码！",QMessageBox::Ok);
        ui->line_UserPasswd->setFocus();
    }else{
        userName=ui->line_UserName->text();
        passwd=ui->line_UserPasswd->text();

        bool ok=false;
        for(int i=0;i<model->rowCount();i++){
            QSqlRecord record =model->record(i);
            if(userName==record.value("username").toString()&&
                    passwd==record.value("userpasswd").toString()){
                    index=i;
                    ok=true;
                    break;
            }
        }

        if(ok){
            QDialog::accept();
        }else{
            QMessageBox::warning(this,"错误","账号或密码错误，请重新输入！");

            ui->line_UserName->clear();
            ui->line_UserPasswd->clear();
            ui->line_UserName->setFocus();
        }
    }
}
void login::registButtonClicked(){
    this->hide();
    CommonRegister rg;
    rg.exec();
    this->show();
}
int login::reIndex(){
    return index;
}
