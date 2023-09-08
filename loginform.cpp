#include "loginform.h"
#include "ui_loginform.h"
#include "registerform.h"
#include "listform.h"
#include<QMessageBox>

loginform::loginform(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginform)
{
    ui->setupUi(this);




    ui->name_edit->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");
    ui->pwd_edit->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");

   this->setStyleSheet("background-color: white;");

    ui->login_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed

    ui->register_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");//pressed

    ui->log_lable->setStyleSheet("QLabel{background:white;}"
                               "QLabel{color:rgb(244,96,108,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                               "QLabel:hover{color:rgb(100,100,100,120);}");
    ui->pwd_label->setStyleSheet("QLabel{background:white;}"
                                 "QLabel{color:rgb(100,100,100,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                                 "QLabel:hover{color:rgb(100,100,100,120);}");
    ui->name_label->setStyleSheet("QLabel{background:white;}"
                                  "QLabel{color:rgb(100,100,100,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                                  "QLabel:hover{color:rgb(100,100,100,120);}");



}

loginform::~loginform()
{
    delete ui;
}


void loginform::on_register_Button_clicked()
{
    //跳转注册页
   registerForm* register_form= new registerForm(parentWidget());
   register_form->show();
   this->close();
}

void loginform::on_login_Button_clicked()
{

    QString username=ui->name_edit->text();
    QString userpwd=ui->pwd_edit->text();

    if(username=="")
    {
        QMessageBox::warning(this, "Warning!", "账号不能为空！", QMessageBox::Ok);
    }
    else if(userpwd=="")
    {
        QMessageBox::warning(this, "Warning!", "请输入密码！", QMessageBox::Ok);
    }
    else
    {
        client=new QTcpSocket();
        client->connectToHost(ip,port);
        if(!client->waitForConnected()){
            qDebug("连接不成功");
            QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
            return;
        }

        QString ms=QString("CheckLogin##%1##%2##127.0.0.1").arg(username).arg(userpwd);
        client->write(ms.toUtf8());
        client->flush();
        connect(client,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = client->readAll();
            qDebug()<<"click";
            qDebug()<<QString(buffer);
            if(QString(buffer).section("##",0,0)=="wrong")
            {//账号密码有错
                QMessageBox::warning(this, "Warning!", "请输入正确的账号和密码！", QMessageBox::Ok);
            }
            else
            { //服务器判断账号密码无误
                QString user_name=QString(buffer).section("##",1,1);
                QString user_ip=QString(buffer).section("##",2,2);
                QString user_sign=QString(buffer).section("##",3,3);
                int image_Id=QString(buffer).section("##",4,4).toInt();
                QMessageBox::information(this, "提示!", "登录成功", QMessageBox::Ok);
                ListForm* ls=new ListForm(parentWidget());
                ls->rcvLogin(ui->name_edit->text(),user_name,user_ip,user_sign,image_Id);//该函数包含了show
                this->close();
            }
            client->close();
        });
    }

}

void loginform::loginClose()
{
    this->close();
}
