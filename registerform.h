#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class registerForm;
}

class registerForm : public QDialog
{
    Q_OBJECT

public:
    explicit registerForm(QWidget *parent = nullptr);
    ~registerForm();

private slots:
    void on_false_Button_clicked();

    void on_true_Button_clicked();

private:
    Ui::registerForm *ui;
    QTcpSocket *client;
    QString ip="127.0.0.1";//服务器ip
    int port=22222;//端口
};

#endif // REGISTERFORM_H
