#include<iostream>
#include"mainwindow.h"
#include"mysplashscreen.h"
#include<QApplication>
#include<QMovie>
#include"unistd.h"
#include"main.h"
using namespace std;
using namespace cv;

Ui::MainWindow *ui = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   //定义一个Qt应用程序对象，是Qt图形界面程序的入口
    QPixmap pixmap("C:\\Users\\haitang\\Pictures\\Saved Pictures\\num.gif");
    MySplashScreen splash(pixmap, 3000);
    QLabel label(splash.w);
    QMovie mv("C:\\Users\\haitang\\Pictures\\Saved Pictures\\num.gif");
    label.setMovie(&mv);
    mv.start();
    splash.show();
    splash.setCursor(Qt::BlankCursor);
    for(int i=0; i<100; i++){
        a.processEvents();
        usleep(30000);
    }
    MainWindow win;
    win.show();
    splash.finish(&win);
    return a.exec();      //进入Qt应用程序的事件循环函数等待用户操作和系统的消息然后进行处理
}





