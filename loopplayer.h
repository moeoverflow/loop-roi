#ifndef LOOPPLAYER_H
#define LOOPPLAYER_H

#include <vector>
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>

#include "loopdata.h"

class LoopPlayer : public QThread
{   Q_OBJECT
private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    QImage img;

    LoopData * loopData = nullptr;

    int width = 0;
    int height = 0;

    void updateFrames();

signals:
    void processedImage(const QImage &image);

protected:
    void run();
    void msleep(int ms);
public:

    LoopPlayer(QObject *parent = nullptr);
    ~LoopPlayer();

    void loadVideo(std::string filename);

    void play();
    void pause();
    bool isStopped() const;

    void setPoints(QVector<QPoint> * points);

    LoopData * getLoopData();

    void setWidth(int width);
    void setHeight(int height);
};

#endif // LOOPPLAYER_H
