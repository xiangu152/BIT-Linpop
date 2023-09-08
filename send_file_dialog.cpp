#include "send_file_dialog.h"
#include "ui_send_file_dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

send_file_Dialog::send_file_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::send_file_Dialog)
{
    ui->setupUi(this);
    connect(&timer,&QTimer::timeout,[=](){
        timer.stop();
        sendData();
    });

    int sendport = 19988;
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress(userIp),sendport);

    connect(tcpServer,&QTcpServer::newConnection,[=](){
        tcpSocket = tcpServer->nextPendingConnection();
        QString ip = tcpSocket->peerAddress().toString().section(":",3,3);
        QString showmessage = ip + "已连接";
        ui->send_Button->setEnabled(true);
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            qDebug()<<buffer;
            if("send_ok" == QString(buffer)){
                this->close();
            }
        });
    });
    ui->send_Button->setEnabled(false);
}

send_file_Dialog::~send_file_Dialog()
{
    delete ui;
}

void send_file_Dialog::setDate(QString userid,QString userip,QString id,QString ip)
{
    this->userId=userid;
    this->userIp=userip;
    this->friendId=id;
    this->friendIp=ip;
}

void send_file_Dialog::on_select_file_Button_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,"选择文件","../");
    if(filePath.isEmpty())
    {
        QMessageBox::information(this,"Error: ",filePath+"文件路径无效");
        return;
    }
    fileName.clear();
    fileSize = 0;
    sendSize = 0;
    QFileInfo info(filePath);
    fileName = info.fileName();
    fileSize = info.size();
    file.setFileName(filePath);
    ui->file_name_label->setText(filePath);
    bool isOK = file.open(QIODevice::ReadOnly);
    if(!isOK)
    {
        QMessageBox::information(this,"Error:","只读方式打开文件失败");
        return;
    }
    else
    {
        ui->file_name_label->setText("选择文件成功："+filePath);
        ui->select_file_Button->setEnabled(false);
    }
}

void send_file_Dialog::on_send_Button_clicked()
{
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    qint64 len = tcpSocket->write(head.toUtf8());
    if(len > 0)
        timer.start(20);
    else
    {
        QMessageBox::information(this,"Error:","头部信息发送失败");
        file.close();
        return;
    }
}

void send_file_Dialog::sendData()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(fileSize);
    quint64 len = 0;
    do
    {
        char buf[1024] = {0};
        len = 0;
        len = file.read(buf,sizeof(buf));
        len = tcpSocket->write(buf,len);
        sendSize += len;
        ui->progressBar->setValue(sendSize);
    }
    while(len >0);
    QMessageBox::information(this,"Success:","发送完毕");
    this->close();
}

void send_file_Dialog::closeEvent(QCloseEvent *event)
{
    QString hostip = "127.0.0.1";
    int hosthost = 22222;
    tcpSocket = new QTcpSocket();
    tcpSocket->abort();
    tcpSocket->connectToHost(hostip, hosthost);
    if(!tcpSocket->waitForConnected(30000))
    {
        this->close();
    }
    else
    {
        QString loginmessage = QString("send_file_miss##%1##%2").arg(userId).arg(friendId);
        qDebug()<<loginmessage;
        tcpSocket->write(loginmessage.toUtf8());
        tcpSocket->flush();
    }
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpServer->disconnect();
    tcpServer->close();
}

