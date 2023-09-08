#include "registerform.h"
#include "ui_registerform.h"
#include<QMessageBox>
#include "loginform.h"
registerForm::registerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerForm)
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
    ui->pwd_again_edit->setStyleSheet("font: 25 14pt '微软雅黑 Light';" //字体
                                    "color: rgb(31,31,31);"		//字体颜色
                                    "padding-left:20px;"       //内边距-字体缩进
                                    "background-color: rgb(255, 255, 255);" //背景颜色
                                    "border:2px solid rgb(20,196,188);border-radius:15px;");
    this->setStyleSheet("background-color: white;");

    ui->name_label->setStyleSheet("QLabel{background:white;}"
                                  "QLabel{color:rgb(100,100,100,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                                  "QLabel:hover{color:rgb(100,100,100,120);}");
    ui->pwd_label->setStyleSheet("QLabel{background:white;}"
                                 "QLabel{color:rgb(100,100,100,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                                 "QLabel:hover{color:rgb(100,100,100,120);}");
    ui->pwd_label_2->setStyleSheet("QLabel{background:white;}"
                                   "QLabel{color:rgb(100,100,100,250);font-size:25px;font-weight:bold;font-family:Roman times;}"
                                   "QLabel:hover{color:rgb(100,100,100,120);}");

    ui->true_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                     "border: none;border-radius:15px;}"
                                     "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                     "QPushButton:pressed{background-color: rgb(17,171,164);}");
    ui->false_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                    "border: none;border-radius:15px;}"
                                    "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                    "QPushButton:pressed{background-color: rgb(17,171,164);}");

}

registerForm::~registerForm()
{
    delete ui;
}

void registerForm::on_false_Button_clicked()
{
    //取消注册
    loginform *lg=new loginform(parentWidget());
    lg->show();
    this->close();
}

void registerForm::on_true_Button_clicked()
{
    //获取用户信息
    QString username=ui->name_edit->text();
    QString userpwd=ui->pwd_edit->text();
    QString userpwd_again=ui->pwd_again_edit->text();


    //else
    if(username=="")
    {
        QMessageBox::warning(this, "Warning!", "账号不能为空！", QMessageBox::Yes);
    }
    else if(userpwd=="")
    {
        QMessageBox::warning(this, "Warning!", "请输入密码！", QMessageBox::Yes);
    }
    else if(userpwd!=userpwd_again)
    {
        QMessageBox::warning(this, "Warning!", "前后密码不一致！", QMessageBox::Yes);
    }
    else
    {//向服务器发送命令
        //注册判断函数
        //查询密码是否合规，账号是否重复
        //发送注册信息
        client=new QTcpSocket();
        client->connectToHost(ip,port);
        if(!client->waitForConnected()){
            qDebug("连接不成功");
            QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
            return;
        }

        QString msg=QString("Register##%1##%2##127.0.0.1").arg(username).arg(userpwd);
        client->write(msg.toUtf8());
        client->flush();

        //接收到返回消息时
        connect(client,&QTcpSocket::readyRead,this,[=]{
            QString msg=client->readAll();
            qDebug()<<msg;
            if(msg=="wrong")
            {//账号已被注册，占用
                QMessageBox::warning(this, "Warning!", "该账号已被使用！", QMessageBox::Yes);
            }
            else
            { //服务器判断账号密码无误
                QMessageBox::information(this, "提示!", "注册成功", QMessageBox::Yes);
                on_false_Button_clicked();//关闭窗体，回到登陆界面
            }
            client->close();
        });
    }
}

