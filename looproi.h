#ifndef LOOPROI_H
#define LOOPROI_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class LoopRoi
{
public:
    LoopRoi();

    bool drawing = false;
    QVector<QPoint> * points;
    int width = 0;
    int height = 0;

    bool reverse = false;

    std::vector<cv::Mat> generateRoiFrames(std::vector<cv::Mat> frames, QVector<QPoint> points);

private:
    bool pointInPolygon(QPoint point, QVector<QPoint> polygon);
};

#endif // LOOPROI_H
