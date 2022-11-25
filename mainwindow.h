#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include <QMainWindow>

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>

#include<QDate>
#include<QDebug>
#include<QTime>
#include<QString>
#include<QDateTime>

#include"dialog.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage  Mat2QImage(Mat cvImg);
    QString getphoto();
    int getPress();
    int isPress;

private slots:
    void openCamara();      // 打开摄像头
    void readFarme();       // 读取当前帧信息
    void closeCamara();     // 关闭摄像头。
    void takingPictures();  // 拍照


private:
    Ui::MainWindow *ui;

    QTimer          *timer;
    QImage          imag;
    Mat             cap,cap_gray,cap_tmp; //定义一个Mat变量，用于存储每一帧的图像
    VideoCapture    capture; //声明视频读入类

    /* 保存路径*/
    QString runPath;
    QString hglpName;
    QString hglpPath;
    QString photoPath;
    QString photoName;
    QString DcardXuehao;
    QString DcardPath;
    QString Dcard;
    QString DcardLabel;
    QString label;
    QString LabelPath;

signals:
void SignalParameter(QImage g);
};
#endif // MAINWINDOW_H

