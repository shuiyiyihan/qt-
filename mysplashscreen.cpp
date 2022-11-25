#include "mysplashscreen.h"
#include <QDateTime>
#include <QProgressBar>
#include <QTimer>
#include <QDebug>
#include "unistd.h"

MySplashScreen::MySplashScreen(QPixmap &pixmap, int time): QSplashScreen(pixmap),
    elapseTime(time)
{
    w = new QWidget(this);
        w->setGeometry(0,0, 600, 550);
        progressBar = new QProgressBar(this);
        progressBar->setGeometry(16, w->height()-100, w->width()-16, 30);
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        progressBar->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        generateAscendRandomNumber();
        setProgress();
}

MySplashScreen::~MySplashScreen(){}

void MySplashScreen::setProgress()
{
    int tempTime = elapseTime/100;
        for (int i=0; i<100; i++) {
            QTimer::singleShot(i*tempTime, this, SLOT(updateProgress()));
        }
        QTimer::singleShot(elapseTime, this, SLOT(close()));
}

void MySplashScreen::generateAscendRandomNumber()
{
    int i;
    qsrand(static_cast<uint>(QTime(0,0,0).secsTo(QTime::currentTime())));
    for (i=0; i<100; i++) {
        numberList.append(qrand()%101);
    }
    std::sort(numberList.begin(), numberList.end());
    //qDebug() << "生成数字完毕: " << numberList;
}

void MySplashScreen::updateProgress()
{
    static int num=0;
    progressBar->setValue(numberList[num]);
    //qDebug()<< numberList[num];
    num++;
}
