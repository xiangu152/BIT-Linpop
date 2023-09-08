#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class loginform; }
QT_END_NAMESPACE

class loginform : public QMainWindow
{
    Q_OBJECT

public:
    loginform(QWidget *parent = nullptr);
    ~loginform();

private slots:
    void on_register_Button_clicked();

    void on_login_Button_clicked();

signals:
    void  userLogin(QString,QString,QString,QString,int);
public slots:
    void loginClose();

private:
    Ui::loginform *ui;
    QTcpSocket *client;
    QString ip="127.0.0.1";//服务器ip
    int port=22222;//端口
};
#endif // LOGINFORM_H
