#ifndef LISTFORM_H
#define LISTFORM_H

#include <QMainWindow>
#include <QtNetwork>
#include <QListWidgetItem>

namespace Ui {
class ListForm;
}

class ListForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListForm(QWidget *parent = nullptr);
    ~ListForm();
    QString userid;
    QString username;
    QString userip;
    QString usersign;
    int imageId;

signals:
    void closeLogin();
    void send_chat();
    void user_begin();
    void newfrd();

public slots:
    void rcvLogin(QString,QString,QString,QString,int);
    void user_get();
    void rcvImage(int);
private slots:

    void rcvFriendId(QString,QString);
    void  list();
    void rcvUsers(QString,QString,QString);

    void on_add_Button_clicked();
    void on_del_Button_clicked();
    void closeEvent(QCloseEvent* event);
    void send_change(QString,QString,QString,int);
   // void test(QString,QString);
  //  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);



    void on_setting_Button_clicked();

    void on_pic_Button_clicked();

private:
    Ui::ListForm *ui;
    QTcpSocket *tcpSocket;
    QString ip="127.0.0.1";
    int port=22222;
};

#endif // LISTFORM_H
