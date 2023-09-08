#ifndef RCV_FILE_DIALOG_H
#define RCV_FILE_DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QFile>
#include <QCloseEvent>

namespace Ui {
class rcv_file_Dialog;
}

class rcv_file_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit rcv_file_Dialog(QWidget *parent = nullptr);
    ~rcv_file_Dialog();

    void closeEvent(QCloseEvent *event);
    void setDate(QString,QString,QString,QString);

private:
    Ui::rcv_file_Dialog *ui;
    QTcpSocket *tcpSocket;
    QFile file;
    QString fileName;
    quint64 fileSize;
    quint64 receiveSize;
    bool isStart;
    QString userId;
    QString userIP;
    QString friendId;
    QString friendIP;
   // QString ip;
    qint16 port;
};

#endif // RCV_FILE_DIALOG_H
