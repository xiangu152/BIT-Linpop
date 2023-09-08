#ifndef CITEMWIDGET_H
#define CITEMWIDGET_H

#include <QWidget>

namespace Ui {
class CItemWidget;
}

class CItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CItemWidget(QWidget *parent = nullptr);
    ~CItemWidget();
    void setItem(QString,QString,QString,QString,int,int,int,QString,QString,QString);
    QString name;
    QString id;
    QString ip;
    QString lastText="";
    int is_new;
    int imageId;
    int is_onLine;
    QString userId;
    QString username;
    QString userIP;

signals:
    void getfriendId(QString,QString);
    void friend_chat(QString,QString,QString,QString,QString,QString);
public slots:
     void mouseDoubleClickEvent(QMouseEvent *event);
     void mousePressEvent(QMouseEvent *event);
private:
    Ui::CItemWidget *ui;



};

#endif // CITEMWIDGET_H
