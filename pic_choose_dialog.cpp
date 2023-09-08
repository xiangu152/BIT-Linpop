#include "pic_choose_dialog.h"
#include "ui_pic_choose_dialog.h"

pic_choose_Dialog::pic_choose_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pic_choose_Dialog)
{
    ui->setupUi(this);
}

pic_choose_Dialog::~pic_choose_Dialog()
{
    delete ui;
}

void pic_choose_Dialog::on_Button_1_clicked()
{
    imageId=1;
}

void pic_choose_Dialog::on_Button_2_clicked()
{
     imageId=2;
}

void pic_choose_Dialog::on_Button_3_clicked()
{
     imageId=3;
}

void pic_choose_Dialog::on_Button_4_clicked()
{
     imageId=4;
}

void pic_choose_Dialog::on_Button_5_clicked()
{
     imageId=5;
}

void pic_choose_Dialog::on_Button_6_clicked()
{
     imageId=6;
}

void pic_choose_Dialog::on_Button_7_clicked()
{
     imageId=7;
}

void pic_choose_Dialog::on_Button_8_clicked()
{
     imageId=8;
}


void pic_choose_Dialog::on_true_Button_clicked()
{
    emit get_Image(imageId);
    this->close();
}

void pic_choose_Dialog::on_false_Button_clicked()
{
    this->close();
}
