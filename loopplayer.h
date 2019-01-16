#ifndef LOOPPLAYER_H
#define LOOPPLAYER_H

#include <vector>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class LoopPlayer : public QThread
{   Q_OBJECT
private:
    cv::VideoCapture capture;
    cv::Mat frame;
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    std::vector<cv::Mat> frames;
    double frameRate;
    QImage img;

    QVector<QPoint> points;
    QVector<QPoint> selectedPoints;

    void updateFrames();

signals:
    void processedImage(const QImage &image);

protected:
    void run();
    void msleep(int ms);
public:
    int width;
    int height;

    LoopPlayer(QObject *parent = 0);
    ~LoopPlayer();
    bool loadVideo(std::string filename);
    void play();
    void pause();
    bool isStopped() const;

    void setPoints(QVector<QPoint> * points);
};

#endif // LOOPPLAYER_H
