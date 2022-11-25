#ifndef MYSPLASHSCREEN_H
#define MYSPLASHSCREEN_H
#include <QProgressBar>
#include <QSplashScreen>

class MySplashScreen : public QSplashScreen
{
    Q_OBJECT;
public:
    MySplashScreen(QPixmap &pixmap, int time);
    ~MySplashScreen();
    void setProgress();
    void generateAscendRandomNumber();
        QWidget *w;
    private:
        QProgressBar *progressBar;
        QList<int> numberList;
        int elapseTime;
    private slots:
        void updateProgress();
};

#endif // MYSPLASHSCREEN_H
