#include "rcv_file_dialog.h"
#include "ui_rcv_file_dialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qdir.h>

rcv_file_Dialog::rcv_file_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rcv_file_Dialog)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->label_message->setText("等待连接");

    port = 19988;
    isStart = true;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,&QTcpSocket::connected,[=](){
        ui->label_message->setText("连接成功，等待发送");
    });

    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        QByteArray buf = tcpSocket->readAll();
        if(isStart == true)
        {
            isStart = false;
            fileName = QString(buf).section("##",0,0);
            fileSize = QString(buf).section("##",1,1).toInt();
            ui->label_message->setText("发现文件"+fileName);
            receiveSize = 0;
            file.setFileName("./"+fileName);
            bool isOK = file.open(QIODevice::WriteOnly|QIODevice::Append);
            if(isOK == false)
            {
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
                QMessageBox::information(this,"Error:","打开文件错误");
                return;
            }
            ui->progressBar->setMinimum(0);
            ui->progressBar->setMaximum(fileSize);
            ui->progressBar->setValue(0);
        }
        else
        {

            ui->label_message->setText("正在接收文件");
            quint64 len = file.write(buf);
            if(len > 0)
            {
                receiveSize += len;
            }
            ui->progressBar->setValue(receiveSize);
            if(receiveSize == fileSize)
            {
                file.close();
                ui->label_message->setText("成功接收文件");
                QString filename;
                QWidget *qwidget = new QWidget();
                filename = QFileDialog::getSaveFileName(qwidget,"choose file","./"+fileName,nullptr);
                QDir d;
                d.rename("./"+fileName,filename);
                this->close();
                ui->progressBar->setValue(0);
            }
        }
    });
}

rcv_file_Dialog::~rcv_file_Dialog()
{
    delete ui;
}

void rcv_file_Dialog::setDate(QString user_id, QString user_ip, QString friend_id, QString friend_ip)
{
    this->userId=user_id;
    this->userIP=user_ip;
    this->friendId=friend_id;
    this->friendIP=friend_ip;

    tcpSocket->connectToHost(friendIP, port);
    qDebug() << friendIP;
}

void rcv_file_Dialog::closeEvent(QCloseEvent *event)
{
    QString hostip = "127.0.0.1";
    int hosthost = 22222;
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();
    tcpSocket->connectToHost(friendIP, port);
    QString message = QString("send_ok");
    tcpSocket->write(message.toUtf8());
    tcpSocket->flush();
    tcpSocket->disconnectFromHost();
    tcpSocket->close();

    tcpSocket = new QTcpSocket();
    tcpSocket->abort();
    tcpSocket->connectToHost(hostip, hosthost);
    if(!tcpSocket->waitForConnected(30000))
    {
        this->close();
    }
    else
    {
        QString loginmessage = QString("send_file_ok##%1##%2").arg(userId).arg(friendId);
        qDebug()<<loginmessage;
        tcpSocket->write(loginmessage.toUtf8());
        tcpSocket->flush();
    }
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}
