#include "dialog.h"
#include "ui_dialog.h"

using namespace cv;
using namespace std;

QString s;
int isPress=0;
int savePress;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->save_as, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(on_pushButton_3_clicked()));

    this->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(251,102,102, 200), stop:1 rgba(20,196,188, 210));");
    ui->save->setStyleSheet("background-color: white");
    ui->save_as->setStyleSheet("background-color: white");

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
    s=getphoto();
    isPress=getPress();
}

QString Dialog::getphoto(){
    return photoPath;
}
int Dialog::getPress(){
    int save=isPress;
    return save;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::SlotParameter(QImage g){
    g = g.scaled(ui->photo->width(), ui->photo->height(),
                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//设置图片大小和photo窗口一致
    ui->photo->setPixmap(QPixmap::fromImage(g));
}

void Dialog::on_pushButton_2_clicked()        //将图片另存到自己想要的位置
{
    QString filename1 = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)")); //选择路径
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(ui->photo->winId()).save(filename1);
}

int Dialog::on_pushButton_3_clicked()       //保存图片到本地特定的路径
{
    QDateTime time = QDateTime::currentDateTime();
    QString dateTime = time.toString("MM-dd-hh-mm");
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(ui->photo->winId()).save(QString("%1/test.jpg").arg(hglpPath));
    see(photoPath,DcardPath,LabelPath);
//    isPress=1;
//    getPress();
//    return 1;
}
//此处photo原来是label


//以下from lwz
double compare(Mat &src, Mat &model)//遍历两张图的像素，检测每个像素点各自的像素值是否相等，记录相等像素点的个数，除以像素点总个数，即可作为两幅图像的相似度
{
 Mat re_model;
 resize(model, re_model, src.size());
 int rows, cols;
 uchar *src_data, *model_data;
 rows = re_model.rows;
 cols = re_model.cols*src.channels();
 double percentage,same=0.0,different=0.0;

for (int i = 0; i < rows; i++)//遍历图像像素
 {
  src_data = src.ptr<uchar>(i);
  model_data = re_model.ptr<uchar>(i);
  for (int j = 0; j < cols; j++)
  {
   if (src_data[j] == model_data[j])
   {
    same++;//记录像素值相同的个数
   }
   else
   {
    different++;//记录像素值不同的个数
   }
  }
 }
 percentage = same / (same + different);
 return percentage;//返回相似度
}



bool Get_Template(vector<Card>&Card_Temp,QString DcardPath){

    for(int i=0;i<10;i++){
        string filename=DcardPath.toStdString().append("/xuehao/");
        //cout<<"Dcardpath"<<filename<<endl;
        Mat roi=imread(filename.append(to_string(i)).append(".jpg"));
        Mat gray;
        cvtColor(roi, gray, COLOR_BGR2GRAY);
        Mat thresh;
        threshold(gray, thresh, 0, 255, THRESH_BINARY_INV|THRESH_OTSU);
        //轮廓检测
        vector<vector<Point>> contours;
        findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        Rect rect=boundingRect(contours[0]);
        Mat reroi=roi(rect);
        Card_Temp.push_back({reroi,rect});
    }
}
bool Cut_Slice(vector<Card>&Block_ROI,vector<Card>&Slice_ROI){
    for (int i = 0; i < Block_ROI.size(); i++)
    {
        Mat roi_gray;
        cvtColor(Block_ROI[i].mat, roi_gray, COLOR_BGR2GRAY);
        Mat roi_thresh;
        threshold(roi_gray, roi_thresh, 0, 255, THRESH_BINARY|THRESH_OTSU);
        int height=roi_thresh.rows;//高
        int width=roi_thresh.cols*roi_thresh.channels();//宽*通道数=实际宽
        for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    roi_thresh.at<uchar>(i, j) = 255 - roi_thresh.at<uchar>(i, j);   //对每一个像素反转
                }
        }
        //imshow("thresh",roi_thresh);
        vector <vector<Point>> contours;
        findContours(roi_thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (int j = 0; j < contours.size(); j++)
        {
            Rect rect = boundingRect(contours[j]);
            //字符相对于银行卡所在的位置
            Rect roi_rect(rect.x + Block_ROI[i].rect.x, rect.y + Block_ROI[i].rect.y, rect.width, rect.height);
            Mat r_roi = Block_ROI[i].mat(rect);
            Slice_ROI.push_back({ r_roi ,roi_rect });
        }
    }
    for (int i = 0; i < Slice_ROI.size() - 1; i++)
    {
        for (int j = 0; j < Slice_ROI.size() - 1 - i; j++)
        {
            if (Slice_ROI[j].rect.x > Slice_ROI[j + 1].rect.x)
            {
                Card temp = Slice_ROI[j];
                Slice_ROI[j] = Slice_ROI[j + 1];
                Slice_ROI[j + 1] = temp;
            }
        }
    }

//    for (int i = 0; i < Slice_ROI.size(); i++)
//    {
//        imshow(to_string(i), Slice_ROI[i].mat);
//        waitKey(0);
//    }
    return true;
}

bool ReadData(string filename, vector<int>&label)
{
    fstream fin;
    fin.open(filename, ios::in);
    if (!fin.is_open())
    {
        cout << "can not open the file!" << endl;
        return false;
    }
    int data[10] = { 0 };
    for (int i = 0; i < 10; i++)
    {
        fin >> data[i];
    }
    fin.close();
    for (int i = 0; i < 10; i++)
    {
        label.push_back(data[i]);
    }
    return true;
}
bool Template_Matching(vector<Card>&Card_Temp,
    vector<Card>&Block_ROI, vector<Card>&Slice_ROI,
    vector<int>&result_index)
{
    for (int i = 0; i < Slice_ROI.size(); i++)
    {
        //将字符resize成合适大小，利于识别
        resize(Slice_ROI[i].mat, Slice_ROI[i].mat, Size(20, 39), 1, 1, INTER_LINEAR);
//        imshow(to_string(i),Slice_ROI[i].mat);
        Mat gray;
        cvtColor(Slice_ROI[i].mat, gray, COLOR_BGR2GRAY);
//        imshow(to_string(i).append("a"),gray);
        Mat thresh;//同样二值化图像，便于匹配
        threshold(gray, thresh, 0, 255, THRESH_BINARY|THRESH_OTSU);
        //imshow(to_string(i).append("a"),thresh);
        int maxIndex = 0;//最匹配数字的指针
        double Max = 0.0;
        for (int j = 0; j < Card_Temp.size(); j++)
        {
            resize(Card_Temp[j].mat, Card_Temp[j].mat, Size(41, 78), 1, 1, INTER_LINEAR);
            Mat temp_gray;
            cvtColor(Card_Temp[j].mat, temp_gray, COLOR_BGR2GRAY);
            Mat temp_thresh;//二值化模板
            threshold(temp_gray, temp_thresh, 0, 255, THRESH_BINARY|THRESH_OTSU);
//            imshow(to_string(j).append("b"),temp_thresh);
            double per;
            per=compare(thresh,temp_thresh);//获取匹配率
            if(per>Max){//更新最匹配数字
                Max=per;
                maxIndex=j;
            }
//            if(j==9){
//                cout<<Max<<endl;
//            }
        }
        result_index.push_back(maxIndex);//将匹配结果进行保存
    }
    if (result_index.size() != 10)return false;
//    for(int k=0;k<result_index.size();k++){
//        cout<<result_index[k]<<endl;
//    }
    return true;
}
bool Show_Result(Mat src,
    vector<Card>&Block_ROI,
    vector<Card>&Slice_ROI,
    vector<int>&result_index,QString LabelPath)
{
    //cout<<"LAbelPath"<<LabelPath.toStdString().append("/label.txt")<<endl;
    //读取label标签
    vector<int>label;
    if (!ReadData(LabelPath.toStdString().append("/label.txt"), label))return false;
    //将匹配结果进行显示
    for (int i = 0; i < Block_ROI.size(); i++)
    {
        rectangle(src, Rect(Block_ROI[i].rect.tl(), Block_ROI[i].rect.br()), Scalar(0, 255, 0), 2);
    }
    for (int i = 0; i < Slice_ROI.size(); i++)
    {
        cout << label[result_index[i]] << " ";
        putText(src, to_string(label[result_index[i]]), Point(Slice_ROI[i].rect.tl()), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
    }
    imshow("Demo", src);
    waitKey(0);
    destroyAllWindows();
    return true;
}
int see(QString photoPath,QString DcardPath,QString LabelPath){
    vector<Card>Block_ROI;
    vector<Card>Slice_ROI;
    vector<Card>Card_Temp;

    string filename =s.toStdString().append("/test.jpg");

    //cout<<"filename是"<<filename<<endl;
    Get_Template(Card_Temp,DcardPath);

//    string filename=DcardPath.toStdString().append("//");
//    Mat roi=imread(filename.append(to_string(i)).append(".jpg"));
   // string filename=photoPath.toStdString();

    Mat src=imread(filename);
 //imshow("bbb",src);

    Mat gray;//灰度
    cvtColor(src,gray,COLOR_BGR2GRAY);

    Mat gaussian;//对图像进行高斯滤波，去除噪声
    GaussianBlur(gray, gaussian, Size(3, 3), 0);

    Mat thresh;//图像二值化
    threshold(gaussian, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);

    //下面反转黑白色
    int height=thresh.rows;//高
    int width=thresh.cols*thresh.channels();//宽*通道数=实际宽
    for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                thresh.at<uchar>(i, j) = 255 - thresh.at<uchar>(i, j);   //对每一个像素反转
            }
    }

    Mat close;
    Mat kernel2=getStructuringElement(MORPH_RECT,Size(25,25));//获取结构化元素
    morphologyEx(thresh,close,MORPH_CLOSE,kernel2);//膨胀腐蚀操作

    vector<vector<Point>>contours;
    findContours(close,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);//寻找图形中的轮廓
    for (int i = 0; i < contours.size(); i++)
    {
        //通过面积、长宽比筛选出银行卡号区域
        double area = contourArea(contours[i]);//计算轮廓的面积

        if (area > 8000 && area < 10500)//面积筛选
        {
            Rect rect = boundingRect(contours[i]);
            float ratio = double(rect.width) / double(rect.height);

            if (ratio > 8 && ratio < 12)//长宽比筛选
            {
                Mat ROI = src(rect);
                Block_ROI.push_back({ ROI ,rect });
            }
        }
    }

    Cut_Slice(Block_ROI,Slice_ROI);//将整串数字的图像进行切片

    vector<int>result_index;
    if (Template_Matching(Card_Temp, Block_ROI, Slice_ROI, result_index))
    {
        cout<<"匹配成功"<<endl;
        Show_Result(src, Block_ROI, Slice_ROI, result_index,LabelPath);
    }
    else
    {
        cout << "识别失败！" << endl;
        system("pause");
        return -1;
    }

    waitKey();
    system("pause");
}
