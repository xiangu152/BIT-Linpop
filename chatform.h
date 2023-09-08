#ifndef CHATFORM_H
#define CHATFORM_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>

namespace Ui {
class chatform;
}

class chatform : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatform(QWidget *parent = nullptr);
    ~chatform();
    QString name;
    QString id;
    QString ip;
    QString userId;
    QString username;
    QString userIP;
    QFile file;
    QTimer timer;
    QString fileName;
    quint64 fileSize;
    quint64 sendSize;

public slots:
    void revFriendChat(QString,QString,QString,QString,QString,QString);
private slots:
    void on_send_message_Button_clicked();
    void getchathistory();
    void on_send_file_Button_clicked();
    //void on_select_file_Button_clicked();



    void on_rcv_file_Button_clicked();

private:
    Ui::chatform *ui;

     QString chat_message;

};

#endif // CHATFORM_H
