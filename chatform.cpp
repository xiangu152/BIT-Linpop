#include "chatform.h"
#include "ui_chatform.h"
#include <QTcpSocket>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QDateTime>
#include <QFileInfo>
#include <QFileDialog>
#include "send_file_dialog.h"
#include "rcv_file_dialog.h"
#include "thread"

QString filePath;
int rate=0;

chatform::chatform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatform)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");
    ui->send_message_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(0,0,0);background-color: rgb(25,202,173);"
                                           "border: none;border-radius:15px;}"
                                           "QPushButton:hover{background-color: rgb(140,199,181);}");
    ui->rcv_file_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(0,0,0);background-color: rgb(25,202,173);"
                                       "border: none;border-radius:15px;}"
                                       "QPushButton:hover{background-color: rgb(140,199,181);}");
    ui->send_file_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(0,0,0);background-color: rgb(25,202,173);"
                                        "border: none;border-radius:15px;}"
                                        "QPushButton:hover{background-color: rgb(140,199,181);}");






   // connect(this,SIGNAL(revFriendChat(QString,QString,QString,QString,QString,QString)),this,SLOT(getchathistory()));
}

chatform::~chatform()
{
    delete ui;
}

void chatform::on_send_message_Button_clicked()
{//发送消息
    QTcpSocket* tcpSocket=new QTcpSocket();
    QString hostip = "127.0.0.1";
    int hosthost = 22222;
    if(ui->send_Edit->toPlainText()!="")
        {
            QString text = ui->send_Edit->toPlainText();
            tcpSocket->abort();
            tcpSocket->connectToHost(hostip,hosthost);
            if(!tcpSocket->waitForConnected(30000))
            {
                QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
               // this->close();
            }
            else
            {
                QString message = QString("SEND_MESSAGE##%1##%2##%3").arg(userId).arg(id).arg(text);
                tcpSocket->write(message.toUtf8());
                tcpSocket->flush();
                ui->send_Edit->clear();
//                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
//                     QByteArray buffer = tcpSocket->readAll();
//                     if(QString(buffer).section("##",0,0)==QString("receive_ok"))
//                     {
//                          ui->send_Edit->clear();
//                     }
//                     else
//                     {
//                         QMessageBox::warning(this, "Warning!", "网络波动，请重新发送！", QMessageBox::Yes);
//                     }
//                });
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning!", "不能发送空消息", QMessageBox::Yes);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        getchathistory();
}

void chatform::on_send_file_Button_clicked()
{//发送文件
    QString hostip = "127.0.0.1";
    int hosthost = 22222;
    QTcpSocket* tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接

    tcpSocket->connectToHost(hostip, hosthost);//链接服务器
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        this->close();
    }
    else
    {//服务器连接成功
        QString message = QString("want_send_file##%1##%2").arg(userId).arg(id);
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
    }
    send_file_Dialog* sf=new send_file_Dialog();
    sf->setDate(userId,userIP,id,ip);
    sf->show();
}

void chatform::revFriendChat(QString name,QString id,QString ip,QString username,QString userid,QString userip)
{
    //好友信息
    this->name=name;
    this->id=id;
    this->ip=ip;
    //自己信息
    this->userIP=userip;
    this->username=username;
    this->userId=userid;

    ui->name_label->setText(name);
    ui->user_name_label->setText(username);
    ui->friend_name_label->setText(name);
    ui->user_ip_label->setText(userIP);
    ui->friend_ip_label->setText(ip);

    getchathistory();
}

void chatform::getchathistory()
{//获得历史记录

QString hostip = "127.0.0.1";
    int hosthost = 22222;
    QTcpSocket *tcpSocket = new QTcpSocket;
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostip, hosthost);//链接服务器

    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        this->close();
    }
    else
    {//服务器连接成功
        QString text = QString("get_History##%1##%2").arg(userId).arg(id);
        tcpSocket->write(text.toUtf8());
        tcpSocket->flush();
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            if(QString(buffer).section("##",0,0)==QString("History_ok"))
            {
                QString msg = "";
                int total = QString(buffer).section("##",1,1).toInt();
                for(int Row = 0;Row < total ;Row++)
                {

                    QDateTime time = QDateTime::fromString( QString(buffer).section("##",Row*3+2,Row*3+2),"yyyy-MM-dd hh:mm:ss");
                    QString timeshow = time.toString("yy.MMM.dd-hh:mm:ss");
                    QString idshow = "";
                    if(QString(buffer).section("##",Row*3+4,Row*3+4)==userId)
                    {//我自己发送的消息
                        idshow = " I：";
                    }
                    else
                    {
                        idshow =" "+ name + "：";
                    }
                    msg ="("+timeshow+")" + idshow + QString(buffer).section("##",Row*3+3,Row*3+3) +"\n" + msg;
                }
                ui->chat_textBrowser->setText(msg);
            }

            else if(QString(buffer).section("##",0,0)==QString("History_error"))
            {
                ui->chat_textBrowser->setText("无消息记录");
            }
        });
    }
}

void chatform::on_rcv_file_Button_clicked()
{
    rcv_file_Dialog *rf = new rcv_file_Dialog();
    rf->setDate(userId,userIP,id,ip);
    rf->show();
}
