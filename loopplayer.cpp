#include "loopplayer.h"

#include <iostream>

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
}

bool LoopPlayer::loadVideo(std::string filename)
{
    capture.open(filename);
    if (capture.isOpened())
    {
        frameRate = capture.get(CV_CAP_PROP_FPS);
        width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
        height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));
        capture.read(frame);
        if (frame.channels() == 3)
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
        }
        capture.set(CV_CAP_PROP_POS_FRAMES, 0);
        return true;
    }

    return false;
}

void LoopPlayer::play()
{
    if (!isRunning())
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
    int delay = static_cast<int>(1000.0 / frameRate);

    cv::Mat frame;
    while (!stop) {
        if (!capture.read(frame)) {
            capture.set(CV_CAP_PROP_POS_FRAMES, 0);
            capture.read(frame);
        }
//            capture.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
//            capture.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
        if (frame.channels() == 3)
        {
            cv::cvtColor(frame, frame, CV_BGR2RGB);
        }

        cv::Mat displayFrame = this->frame.clone();
        for (auto selectedPoint : this->selectedPoints) {
            cv::Rect rect(selectedPoint.x(), selectedPoint.y(), 1, 1);
            displayFrame(rect).setTo(frame(rect));
        }

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

bool PointInPolygon(QPoint point, QVector<QPoint> polygon);

void LoopPlayer::updateFrames() {
//        cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);
//        QVector<QPoint> pointsInside;
    int edgeLeft = frame.cols;
    int edgeRight = 0;
    int edgeTop = frame.rows;
    int edgeBottom = 0;
    for (auto point : this->points) {
       edgeLeft = std::min(point.x(), edgeLeft);
       edgeRight = std::max(point.x(), edgeRight);
       edgeTop = std::min(point.y(), edgeTop);
       edgeBottom = std::max(point.y(), edgeBottom);
    }


    selectedPoints.clear();

    for (int w = edgeLeft; w < edgeRight; ++w) {
        for (int h = edgeTop; h < edgeBottom; ++h) {
            QPoint point(w, h);
            bool isInPolygon = PointInPolygon(point, this->points);
            if (isInPolygon) {
                selectedPoints.append(point);
            }
        }
    }
}

void LoopPlayer::setPoints(QVector<QPoint> * points) {
    this->points = *points;
    this->updateFrames();
}

bool PointInPolygon(QPoint point, QVector<QPoint> polygon) {
  int i, j, nvert = polygon.size();
  if (nvert == 0) return false;
  bool c = false;

  QPoint * points = polygon.data();
  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (points[i].y() >= point.y() ) != (points[j].y() >= point.y()) ) &&
        (point.x() <= (points[j].x() - points[i].x()) * (point.y() - points[i].y()) / (points[j].y() - points[i].y()) + points[i].x())
      )
      c = !c;
  }

  return c;
}
