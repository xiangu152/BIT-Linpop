#ifndef SEND_FILE_DIALOG_H
#define SEND_FILE_DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QCloseEvent>

namespace Ui {
class send_file_Dialog;
}

class send_file_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit send_file_Dialog(QWidget *parent = nullptr);
    ~send_file_Dialog();
    void sendData();
public slots:
    void setDate(QString,QString,QString,QString);
private slots:
    void on_select_file_Button_clicked();

    void on_send_Button_clicked();
    void closeEvent(QCloseEvent *event);
private:
    Ui::send_file_Dialog *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QFile file;
    QTimer timer;
    QString fileName;
    quint64 fileSize;
    quint64 sendSize;
    QString userId;
    QString friendId;
    //
    QString userIp;
    QString friendIp;
};

#endif // SEND_FILE_DIALOG_H
