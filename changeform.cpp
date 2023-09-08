#include "changeform.h"
#include "ui_changeform.h"

changeform::changeform(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeform)
{
    ui->setupUi(this);
}

changeform::~changeform()
{
    delete ui;
}

void changeform::on_buttonBox_accepted()
{
    emit getUsers(ui->name_Edit->text(),ui->ip_Edit->text(),ui->sign_Edit->text());
    this->close();
}

void changeform::setUser(QString name,QString ip,QString sign)
{
    this->name=name;
    this->ip=ip;
    this->sign=sign;

    ui->name_Edit->setText(name);
    ui->ip_Edit->setText(ip);
    ui->sign_Edit->setText(sign);
}

void changeform::on_buttonBox_rejected()
{
    this->close();
}
