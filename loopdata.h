#ifndef LOOPVIDEO_H
#define LOOPVIDEO_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class LoopData : public QObject
{
    Q_OBJECT
public:
    explicit LoopData(std::string filename, QObject *parent = nullptr);
    ~LoopData();

    std::string filename;

    std::vector<cv::Mat> loadData(bool shouldResize = true);
    double getFPS();
    int getWidth();
    int getHeight();

    void setCurrentFrameIndex(unsigned long index);
    cv::Mat nextFrame();

    std::vector<cv::Mat> getFrames();
    void setFrames(std::vector<cv::Mat> newFrames);

    void setWidth(int width);
    void setHeight(int height);

    void writeFile(std::vector<cv::Mat> frames, std::string filepath);
private:
    /**
     * @brief original file capture
     */
    cv::VideoCapture capture;

    /**
     * @brief loop data
     */
    std::vector<cv::Mat> frames;
    int width = 0;
    int height = 0;
    unsigned long frameCount = 0;
    unsigned long currentFrameIndex = 0;

signals:

public slots:
};

#endif // LOOPVIDEO_H
