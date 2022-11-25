#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QScreen>
#include <QTime>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <fstream>
#include <string>
#include <iostream>
#include <io.h>
#include <QVector>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString getphoto();
    int getPress();
    int isPress;

private slots:
    void SlotParameter(QImage g);
    void on_pushButton_2_clicked();
    int on_pushButton_3_clicked();

private:
    Ui::Dialog *ui;

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
};

//from lwz
struct Card{
    cv::Mat mat;
    cv::Rect rect;
};
bool Get_Template( std::vector<Card>&Card_Temp,QString DcardPath);
bool Cut_Slice(std::vector<Card>&Block_ROI,std::vector<Card>&Slice_ROI);
bool Template_Matching(std::vector<Card>&Card_Temp,
    std::vector<Card>&Block_ROI,
    std::vector<Card>&Slice_ROI,
    std::vector<int>&result_index);

bool Show_Result(cv::Mat src,
    std::vector<Card>&Block_ROI,
    std::vector<Card>&Slice_ROI,
    std::vector<int>&result_index,QString LabelPath);
int see(QString photoPath,QString DcardPath,QString LabelPath);

#endif // DIALOG_H
