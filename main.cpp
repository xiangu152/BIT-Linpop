#include "loginform.h"
#include"listform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loginform login;
    ListForm list;

    login.show();
    QObject::connect(&login,&loginform::userLogin, &list, &ListForm::rcvLogin);
    QObject::connect(&list, &ListForm::closeLogin, &login, &loginform::loginClose);
    return a.exec();
}
