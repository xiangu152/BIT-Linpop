#ifndef PIC_CHOOSE_DIALOG_H
#define PIC_CHOOSE_DIALOG_H

#include <QDialog>

namespace Ui {
class pic_choose_Dialog;
}

class pic_choose_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit pic_choose_Dialog(QWidget *parent = nullptr);
    ~pic_choose_Dialog();
    int imageId;
signals:
    void get_Image(int);

private slots:
    void on_Button_1_clicked();

    void on_Button_2_clicked();

    void on_Button_3_clicked();

    void on_Button_4_clicked();

    void on_Button_5_clicked();

    void on_Button_6_clicked();

    void on_Button_7_clicked();

    void on_Button_8_clicked();

    void on_true_Button_clicked();

    void on_false_Button_clicked();

private:
    Ui::pic_choose_Dialog *ui;

};

#endif // PIC_CHOOSE_DIALOG_H
