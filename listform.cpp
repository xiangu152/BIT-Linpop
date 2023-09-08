#include "listform.h"
#include "ui_listform.h"
#include "citemwidget.h"
#include "loginform.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QInputDialog>
#include "changeform.h"
#include "pic_choose_dialog.h"
#include <thread>

bool is_open_chatdialog;
int onlinenum = -1;
int listnum = -1;
int sendmassagenum = -1;
int sendfilenum = -1;

QString friendId;
QString friendname;

QString hostip = "127.0.0.1";
int hosthost = 22222;

QList <QString> friendlist;
QList <QString> friendiplist;
QList <QString> friendstatuelist;//1在线
QList <QString> friendsendmassagelist;
QList <QString> friendsendfilelist;

ListForm::ListForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ListForm)
{

    ui->setupUi(this);
    ui->listWidget->setStyleSheet("background-color: rgb(250, 249, 222);");
    this->setStyleSheet("background-color: white;");
    ui->add_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(236,173,158);"
                                  "border: none;border-radius:15px;}"
                                  "QPushButton:hover{background-color: rgb(244,96,108);}");
    ui->del_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(236,173,158);"
                                  "border: none;border-radius:15px;}"
                                  "QPushButton:hover{background-color: rgb(244,96,108);}");
    ui->setting_Button->setStyleSheet("QPushButton{font: 25 14pt '微软雅黑 Light';color: rgb(255,255,255);background-color: rgb(236,173,158);"
                                      "border: none;border-radius:15px;}"
                                      "QPushButton:hover{background-color: rgb(244,96,108);}");
//    connect(this,&ListForm::newfrd,this,&ListForm::list);
}

ListForm::~ListForm()
{
    delete ui;
}

void ListForm::rcvLogin(QString id,QString name,QString ip,QString sign,int image)
{

    //加载好友列表
    //ui->name_label->setText(str);
    userid=id;

    username=name;
    userip=ip;
    usersign=sign;
    imageId=image;

    ui->name_label->setText(username);
    ui->sign_label->setText(usersign);
    QString pic = "pic/"+QString::number(imageId)+".png";
    ui->pic_Button->setIcon(QIcon(pic));
    ui->pic_Button->setIconSize(QSize(100,100));
    //userid是账号，用userid向服务器要昵称，ip，头像
    //use "WantFriendsList##userid" to socket
    //
    this->show();
    list();
}



void ListForm::rcvUsers(QString name,QString ip,QString sign)
{
    username=name;
    ui->name_label->setText(username);
    userip=ip;
    usersign=sign;
    ui->sign_label->setText(usersign);

    send_change(username,userip,usersign,imageId);
}

void ListForm::list()
{
    //QTcpSocket* tcpSocket=new QTcpSocket();
    qDebug()<<"list-start";
    QTcpSocket* tcpSocket=new QTcpSocket();
    tcpSocket->abort();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    tcpSocket->connectToHost(ip,port);
    if(!tcpSocket->waitForConnected())
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        loginform* lg=new loginform(parentWidget());
        lg->show();
        this->close();
    }
    else
    {
        qDebug()<<"list-connect";
        QString message = QString("Get_FriendList##%1").arg(userid);
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
        connect(tcpSocket,&QTcpSocket::readyRead,this,[=](){
            qDebug()<<"list-read";
            QByteArray buffer = tcpSocket->readAll();
            qDebug()<<"list"+QString(buffer);
            if(QString(buffer).section("##",0,0)==QString("Get_Friendlist_ok"))
            {
                int all_num=QString(buffer).section("##",1,1).toInt();
                ui->listWidget->clear();

                for(int row=0;row<all_num;row++)
                {
                    qDebug()<<QString::number(row);
                    QString id = QString(buffer).section("##",2+row*8,2+row*8);
                    QString ip = QString(buffer).section("##",3+row*8,3+row*8);
                    QString name = QString(buffer).section("##",4+row*8,4+row*8);
                    int imageId = QString(buffer).section("##",5+row*8,5+row*8).toInt();
                    QString lastText =QString(buffer).section("##",6+row*8,6+row*8);
                    int is_onLine= QString(buffer).section("##",7+row*8,7+row*8).toInt();
                    int is_newMessage =QString(buffer).section("##",8+row*8,8+row*8).toInt();
                    int is_newFile =QString(buffer).section("##",9+row*8,9+row*8).toInt();
                    int is_new=is_newFile+is_newMessage;

                    CItemWidget* pItemWidget = new CItemWidget(this);
                    pItemWidget->setItem(name,id,ip,lastText,is_new,imageId,is_onLine,username,userid,userip);
                    //pItemWidget->setItem("111","111","120.111","lastText",1,0,1);

                    QListWidgetItem* pItem = new QListWidgetItem();
                    // QListWidgetItem* p = new QListWidgetItem();
                    pItem->setSizeHint(QSize(340, 70));
                    ui->listWidget->addItem(pItem);
                    // ui->listWidget->addItem(p);
                    ui->listWidget->setItemWidget(pItem, pItemWidget);
                    // connect(pItemWidget,SIGNAL(getfriendId(QString,QString)),this,SLOT(revFriendId(QString,QString)));
                    connect(pItemWidget,SIGNAL(getfriendId(QString,QString)),this,SLOT(rcvFriendId(QString,QString)));
                    // pItemWidget->setItem("122","lastText",1,0,0);
                    //  ui->listWidget->setItemWidget(p, pItemWidget);
                }
                tcpSocket->close();
            }
            else if(QString(buffer).section("##",0,0)==QString("Get_Friendlist_Error"))
            {
                ui->listWidget->clear();
                ui->listWidget->insertItem(0,tr("暂无好友"));
                tcpSocket->close();
            }
        });
    }
}

void ListForm::rcvFriendId(QString n,QString m)
{
    friendId=n;
    friendname=m;
    qDebug()<<friendId;
}

void ListForm::user_get()
{
//    QTcpSocket* tcpSocket=new QTcpSocket();
//    tcpSocket->abort();
//    tcpSocket->connectToHost(hostip,hosthost);
//    if(!tcpSocket->waitForConnected(3000))
//    {

//        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
//        this->close();
//    }
//    else
//    {
//        QString message = QString("WantInformation##%1").arg(userid);
//        tcpSocket->write(message.toUtf8());
//        tcpSocket->flush();
////        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
//            QByteArray buffer = tcpSocket->readAll();
//            qDebug()<<"user"+QString(buffer);
//            if(QString(buffer).section("##",0,0)==QString("getInformation"))
//            {
//                username=QString(buffer).section("##",1,1);
//                userip=QString(buffer).section("##",2,2);
//                usersign=QString(buffer).section("##",3,3);
//                imageId=QString(buffer).section("##",4,4).toInt();

//                ui->name_label->setText(username);
//                ui->sign_label->setText(usersign);
//                QString pic = "pic/"+QString::number(imageId)+".png";
//                ui->pic_Button->setIcon(QIcon(pic));
//                ui->pic_Button->setIconSize(QSize(100,100));
//            }
//            else
//            {
//                QMessageBox::warning(this, "Warning!", "用户信息加载失败", QMessageBox::Ok);
//            }
////        });


//    }
//     tcpSocket->disconnectFromHost();
}

void ListForm::on_add_Button_clicked()
{
    bool ok;
    QString addfriendname = QInputDialog::getText(this, tr("增添联系人"),tr("请输入对方昵称"), QLineEdit::Normal,0,&ok);
    if (ok && !addfriendname.isEmpty())
    {
        //
        if(addfriendname != userid)
        {
            QTcpSocket* tcpSocket = new QTcpSocket();
            tcpSocket->connectToHost(ip, port);//链接服务器
            if(!tcpSocket->waitForConnected())
            {
                QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
                loginform *cli = new loginform();
                cli->show();
                tcpSocket->close();
                this->close();
            }
            else
            {//服务器连接成功
                QString message = QString("Add_Friend##%1##%2##%3").arg(userid).arg(addfriendname).arg(username);
                //send "add_friend##userid##addfriendname"to socket
                tcpSocket->write(message.toUtf8());
                tcpSocket->flush();

                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                    QByteArray buffer = tcpSocket->readAll();
                    qDebug() <<"addfrd-rcv:"<< QString(buffer);
                    if( QString(buffer).section("##",0,0) == "Add_Friend_ok")
                    {//加朋友
                        tcpSocket->close();
                        list();
//                        ListForm* ls=new ListForm(parentWidget());
//                        ls->rcvLogin(userid,username,userip,usersign,imageId);
//                        ls->show();
//                        this->close();
                    }
                    else if( QString(buffer).section("##",0,0) == "add_friend_error")
                    {
                        QMessageBox::warning(this, "Warning!", "添加失败", QMessageBox::Yes);
                    }
                    qDebug()<<"addfrd-closetcp";
                    tcpSocket->close();
                });
            }
        }
        else
        {
            // receive"add_friend_error##NotMe"
            QMessageBox::warning(this, "Warning!", "不能添加自己", QMessageBox::Yes);
        }
    }
}

void ListForm::on_del_Button_clicked()
{
    if(ui->listWidget->currentRow()!=-1)
    {
      //  QString friendname = friendlist.at(ui->listWidget->currentRow());
        QString dlgTitle="提示";
        QString strInfo="您将删除好友："+friendname+"\n确定吗？";
        QMessageBox::StandardButton  defaultBtn=QMessageBox::NoButton;
        QMessageBox::StandardButton result;
        result=QMessageBox::question(this, dlgTitle, strInfo,QMessageBox::Cancel|QMessageBox::Yes,defaultBtn);
        if (result==QMessageBox::Yes)
        {
            QTcpSocket* tcpSocket = new QTcpSocket();
            tcpSocket->abort();//取消已有链接
            tcpSocket->connectToHost(hostip, hosthost);//链接服务器
            if(!tcpSocket->waitForConnected())
            {
                QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
                this->close();
                //user.islogin = false;
                loginform *cli = new loginform();
                cli->show();
            }
            else
            {//服务器连接成功
                QString message = QString("Delete_Friend##%1##%2").arg(userid).arg(friendId);
                //send"delete_friend##userid##friendname"to socket
                tcpSocket->write(message.toUtf8());
                tcpSocket->flush();

                connect(tcpSocket,&QTcpSocket::readyRead,[=](){
                    QByteArray buffer = tcpSocket->readAll();
                    qDebug()<<"deletefrd-read:"<<QString(buffer);
                    //receive"delete_friend_ok##userid##friendname"
                    if( QString(buffer).section("##",0,0) == QString("Delete_Friend_Ok"))
                    {
                        tcpSocket->close();
                        list();
                    }
                });
            }
            //tcpSocket->disconnectFromHost();
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning!", "您未选择联系人", QMessageBox::Yes);
    }
}

void ListForm::closeEvent(QCloseEvent *event)
{
    //timer->stop();
    QTcpSocket* tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostip, hosthost);//链接服务器
    if(!tcpSocket->waitForConnected(30000))
    {
        this->close();
        //user.islogin = false;
    }
    else
    {//服务器连接成功
        QString message = QString("logout##%1").arg(userid);
        //send"logout##userid"to socket
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
        QMessageBox::information(this, "Success", "下线成功", QMessageBox::Ok);
    }
}


void ListForm::on_setting_Button_clicked()
{
    changeform* cha=new changeform();
    connect(cha,SIGNAL(getUsers(QString,QString,QString)),this,SLOT(rcvUsers(QString,QString,QString)));
    cha->setUser(username,userip,usersign);
    cha->show();
}

void ListForm::send_change(QString username,QString userip,QString usersign,int imageId)
{//发送修改信息
    QTcpSocket* tcpSocket = new QTcpSocket();
    tcpSocket->abort();//取消已有链接
    tcpSocket->connectToHost(hostip, hosthost);//链接服务器
    if(!tcpSocket->waitForConnected(30000))
    {
        QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Yes);
        this->close();
    }
    else
    {
        QString message = QString("Change##%1##%2##%3##%4##%5").arg(userid).arg(imageId).arg(username).arg(userip).arg(usersign);
        //send"Change##<usersid>##<Imageid>##<name>##<ip>##<sign>"to socket
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush();
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            QByteArray buffer = tcpSocket->readAll();
            qDebug() << QString(buffer);
            if( QString(buffer).section("##",0,0) == QString("Change_ok"))
            {
                QMessageBox::information(this, "Success", "设置成功", QMessageBox::Ok);
            }
            else
            {
                QMessageBox::warning(this, "Warning!", "网络错误", QMessageBox::Ok);
            }
        });

    }
}

void ListForm::on_pic_Button_clicked()
{//选择图片
    pic_choose_Dialog* p=new pic_choose_Dialog();
    connect(p,SIGNAL(get_Image(int)),this,SLOT(rcvImage(int)));
    p->show();
}

void ListForm::rcvImage(int Image)
{
    if(Image<9&&Image>0)
        imageId=Image;
    qDebug()<<QString::number(Image);
    QString pic = "pic/"+QString::number(imageId)+".png";
    ui->pic_Button->setIcon(QIcon(pic));
    send_change(username,userip,usersign,imageId);
}
