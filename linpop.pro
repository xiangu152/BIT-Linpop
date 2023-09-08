    QT       += core gui
QT += network
QT += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changeform.cpp \
    chatform.cpp \
    citemwidget.cpp \
    listform.cpp \
    main.cpp \
    loginform.cpp \
    pic_choose_dialog.cpp \
    rcv_file_dialog.cpp \
    registerform.cpp \
    send_file_dialog.cpp

HEADERS += \
    changeform.h \
    chatform.h \
    citemwidget.h \
    listform.h \
    loginform.h \
    pic_choose_dialog.h \
    rcv_file_dialog.h \
    registerform.h \
    send_file_dialog.h

FORMS += \
    changeform.ui \
    chatform.ui \
    citemwidget.ui \
    listform.ui \
    loginform.ui \
    pic_choose_dialog.ui \
    rcv_file_dialog.ui \
    registerform.ui \
    send_file_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
