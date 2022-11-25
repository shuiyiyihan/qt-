#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDate>
#include<QDebug>
#include<QTime>
#include<QString>
#include<QDateTime>
#include <opencv2\imgproc\types_c.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<fstream>
#include<string>
#include<iostream>
#include<io.h>
#include "main.h"
#include "dialog.h"
MainWindow::MainWindow(QWidget *parent) :    //MainWindow的构造函数
    QMainWindow(parent),  //显式调用基类的构造函数
    ui(new Ui::MainWindow)  //初始化ui指针
{
    ui->setupUi(this);

    timer   = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
    connect(ui->open, SIGNAL(clicked()), this, SLOT(openCamara()));
    connect(ui->take, SIGNAL(clicked()), this, SLOT(takingPictures()));   //将信号和槽联系起来
    connect(ui->close, SIGNAL(clicked()), this, SLOT(closeCamara()));

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));");
    ui->camera->setStyleSheet("background-color: black");
    ui->take->setStyleSheet("background-color: white");
    ui->open->setStyleSheet("background-color: white");
    ui->close->setStyleSheet("background-color: white");

    runPath = QCoreApplication::applicationDirPath();       //获取exe路径
    hglpName = "photo";
    photoName ="test";
    hglpPath = QString("%1/%2").arg(runPath).arg(hglpName);     //相当于文本替换，用来找到指定路径
    photoPath = QString("%1/%2").arg(runPath).arg(hglpName).arg(photoName);
    DcardXuehao ="xuehao";
    Dcard = "Dcard";
    DcardPath = QString("%1/%2").arg(runPath).arg(Dcard).arg(DcardXuehao);  //同上
    DcardLabel = "label.txt";
    LabelPath = QString("%1/%2").arg(runPath).arg(Dcard).arg(DcardLabel);


}


//打开摄像头
void MainWindow::openCamara()
{
    capture.open(0);    //打开摄像头，从摄像头读入视频如果设备只有一个摄像头就传入参数0
    qDebug("open");
    if (!capture.isOpened()) //先判断是否打开摄像头
    {
         qDebug("没打开摄像头！");
    }
    capture.set(CAP_PROP_FRAME_WIDTH, 2036);     //设置摄像头的宽度和高度
    capture.set(CAP_PROP_FRAME_HEIGHT, 1282);
    timer->start(1);              // 开始计时，1ms获取一帧
}

//读取摄像头信息
void MainWindow::readFarme()
{
    capture>>cap; //读取当前帧
    if (!cap.empty()) //判断当前帧是否捕捉成功 **这步很重要
    {
        imag = Mat2QImage(cap);
        imag = imag.scaled(ui->camera->width(), ui->camera->height(),
                                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//设置图片大小和label的长宽一致
        //imshow(name, cap); //若当前帧捕捉成功，显示
        ui->camera->setPixmap(QPixmap::fromImage(imag));  // 将图片显示到label上
    }
    else
        qDebug("获取当前帧失败");
}
// 拍照
void MainWindow::takingPictures()
{
    capture>>cap; //读取当前帧
    if (!cap.empty()) //判断当前帧是否捕捉成功 **这步很重要
    {
        Dialog d;
        imag = Mat2QImage(cap);
        connect(this, SIGNAL(SignalParameter(QImage)), &d, SLOT(SlotParameter(QImage)));//用于将image传入dialog界面
        emit SignalParameter(imag);
        d.exec();
    }
    else
        qDebug("拍照获取当前帧失败 ");
}


//关闭摄像头，释放资源，必须释放***
void MainWindow::closeCamara()
{
    timer->stop();         // 停止读取数据。
}


// 图片转换
QImage  MainWindow::Mat2QImage(Mat cvImg)  //qt和opencv图片定义和操作有所不同——Mat的主要功能是矩阵运算，
{                                           //拥有数之不尽的数据类型；而QImage侧重于图片显示，更多的操作是在图形界面上。
    QImage qImg;
    if(cvImg.channels()==3)     //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}

MainWindow::~MainWindow()
{
    delete ui;
}
