#include "server.h"
#include<QMediaPlayer>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    server w;
    w.setFixedSize(600,600);
    QMediaPlayer player;
//    player.setMedia(QUrl::fromLocalFile("SHOW.mp3"));

    // 在窗口弹出时播放声音
    QObject::connect(&w, &server::show, [&player]() {
//        player.play();
    });
    w.show();
    return a.exec();
}
