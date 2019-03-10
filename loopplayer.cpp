#include "loopplayer.h"

#include <iostream>

using namespace std;

LoopPlayer::LoopPlayer(QObject *parent) : QThread(parent)
{
    stop = true;
}

LoopPlayer::~LoopPlayer()
{
    mutex.lock();
    stop = true;
    condition.wakeOne();
    mutex.unlock();
    wait();
    delete this->loopData;
}

void LoopPlayer::loadVideo(string filename)
{
    this->pause();
    this->loopData = new LoopData(filename);
}


void LoopPlayer::play()
{
    if (!isRunning() && this->loopData != nullptr)
    {
        if (isStopped())
        {
            stop = false;
        }

        start(LowPriority);
    }
}

void LoopPlayer::run()
{
    while (!stop && this->loopData != nullptr) {
        cv::Mat frame;
        int delay = static_cast<int>(1000.0 / this->loopData->getFPS());
        cv::Mat displayFrame = this->loopData->nextFrame();

        if (displayFrame.channels() == 3)
        {

            img = QImage(reinterpret_cast<const unsigned char*>(displayFrame.data),
                         displayFrame.cols,
                         displayFrame.rows,
                         QImage::Format_RGB888);
        }
        else
        {
            img = QImage(reinterpret_cast<const unsigned char*>(displayFrame.data),
                         displayFrame.cols,
                         displayFrame.rows,
                         QImage::Format_Indexed8);
        }

        emit processedImage(img);
        this->msleep(delay);
    }
}

void LoopPlayer::pause()
{
    stop = true;
}

void LoopPlayer::msleep(int ms)
{
    struct timespec ts =
    {
        ms / 1000, (ms % 1000) * 1000 * 1000
    };

    nanosleep(&ts, nullptr);
}

bool LoopPlayer::isStopped() const
{
    return stop;
}

LoopData * LoopPlayer::getLoopData()
{
    return this->loopData;
}

void LoopPlayer::setWidth(int width)
{
    this->width = width;
    if (this->loopData != nullptr)
    {
        this->loopData->setWidth(width);
    }
}

void LoopPlayer::setHeight(int height)
{
    this->height = height;
    if (this->loopData != nullptr)
    {
        this->loopData->setHeight(height);
    }
}
