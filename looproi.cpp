#include "looproi.h"

using std::vector;
using cv::Mat;
using cv::Point;
using cv::Vec3i;

LoopRoi::LoopRoi()
{
    this->points = new QVector<QPoint>();
}

bool LoopRoi::pointInPolygon(QPoint point, QVector<QPoint> polygon) {
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

vector<Mat> LoopRoi::generateRoiFrames(const vector<Mat> frames, QVector<QPoint> points)
{
    Mat staticFrame = frames[0];

    int edgeLeft = staticFrame.cols;
    int edgeRight = 0;
    int edgeTop = staticFrame.rows;
    int edgeBottom = 0;
    for (auto point : points) {
       edgeLeft = std::min(point.x(), edgeLeft);
       edgeRight = std::max(point.x(), edgeRight);
       edgeTop = std::min(point.y(), edgeTop);
       edgeBottom = std::max(point.y(), edgeBottom);
    }
    edgeRight = std::min(edgeRight, staticFrame.cols);
    edgeBottom = std::min(edgeBottom, staticFrame.rows);

    vector<Mat> roiFrames;
    for (auto frame : frames)
    {
        Mat roiFrame = this->reverse ? frame.clone() : staticFrame.clone();
        roiFrames.push_back(roiFrame);
    }
    for (int w = edgeLeft; w < edgeRight; ++w) {
        for (int h = edgeTop; h < edgeBottom; ++h) {
            QPoint point(w, h);
            Point cvPoint(w, h);
            bool isInPolygon = this->pointInPolygon(point, *this->points);
            if (isInPolygon) {
                for (unsigned long i = 0; i < roiFrames.size(); i++)
                {
                    roiFrames[i].at<cv::Vec3b>(cvPoint) = (this->reverse ? staticFrame : frames[i]).at<cv::Vec3b>(cvPoint);
                }
            }
        }
    }
    return roiFrames;
}
