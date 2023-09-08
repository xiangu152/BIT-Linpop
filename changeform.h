#ifndef CHANGEFORM_H
#define CHANGEFORM_H

#include <QDialog>

namespace Ui {
class changeform;
}

class changeform : public QDialog
{
    Q_OBJECT

public:
    explicit changeform(QWidget *parent = nullptr);
    ~changeform();
    QString name;
    QString ip;
    QString sign;
    void setUser(QString,QString,QString);

signals:
    void getUsers(QString,QString,QString);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::changeform *ui;
};

#endif // CHANGEFORM_H
