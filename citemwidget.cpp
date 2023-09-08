#include "citemwidget.h"
#include "ui_citemwidget.h"
#include <QDebug>
#include<chatform.h>
CItemWidget::CItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CItemWidget)
{
    ui->setupUi(this);
   // connect(this,SIGNAL(setItem(QString,QString,int,int,int)),this,SLOT(load()));
}

CItemWidget::~CItemWidget()
{
    delete ui;
}

void CItemWidget::setItem(QString n,QString i,QString p,QString l,int isnew,int image,int onLine,QString username,QString userid,QString userip)
{
    this->name=n;
    this->id=i;
    this->ip=p;
    this->lastText=l;
    this->is_new=isnew;
    this->imageId=image;
    this->is_onLine=onLine;
    this->userIP=userip;
    this->username=username;
    this->userId=userid;

    ui->name_label->setText(name);
    if(is_new==0)
       ui->isnew_label->hide();
    else
        ui->isnew_label->show();
    ui->lastText_label->setText(lastText);
    if(is_onLine==0)
        ui->name_label->setStyleSheet("color:#f0f0f0;");
    //设置头像
    //
    QString pic = "pic/"+QString::number(imageId)+".png";
    QIcon icon =QIcon(pic);
    ui->image_label->setPixmap(icon.pixmap(icon.actualSize(QSize(65, 65))));
}


 void CItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
 {
     chatform* chat=new chatform();

     connect(this,SIGNAL(friend_chat(QString,QString,QString,QString,QString,QString)),chat,SLOT(revFriendChat(QString,QString,QString,QString,QString,QString)));
     emit friend_chat(name,id,ip,username,userId,userIP);
     chat->show();
 }

 void CItemWidget::mousePressEvent(QMouseEvent *event)
 {
     qDebug()<<"OK";
     emit(getfriendId(id,name));
 }
