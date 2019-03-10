#include "loopdata.h"

using namespace std;
using namespace cv;

LoopData::LoopData(string filename, QObject *parent) : QObject(parent)
{   this->filename = filename;
    this->capture.open(this->filename);
    this->width = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
    this->height = static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

    this->loadData();
}

LoopData::~LoopData()
{
    this->capture.release();
}

void LoopData::loadData()
{
    vector<Mat> frames;

    if (capture.isOpened())
    {
        cv::Mat frame;
        capture.set(CV_CAP_PROP_POS_FRAMES, 0);
        while (capture.read(frame))
        {
            if (frame.channels() == 3)
            {
                cvtColor(frame, frame, CV_BGR2RGB);
            }
            resize(frame, frame, Size(this->width, this->height));
            frames.push_back(frame.clone());
        }
    }
    this->setFrames(frames);
}

double LoopData::getFPS()
{
    return this->capture.get(CV_CAP_PROP_FPS);
}

int LoopData::getWidth()
{
    return this->width;
}

int LoopData::getHeight()
{
    return this->height;
}

void LoopData::setCurrentFrameIndex(unsigned long index)
{
    this->currentFrameIndex = index;
}

cv::Mat LoopData::nextFrame()
{
    this->currentFrameIndex++;
    if (this->currentFrameIndex >= this->frameCount)
    {
        this->currentFrameIndex = 0;
    }
    return this->frames[this->currentFrameIndex];
}

vector<Mat> LoopData::getFrames()
{
    return this->frames;
}

void LoopData::setFrames(vector<Mat> newFrames)
{
    this->frames = newFrames;
    this->frameCount = newFrames.size();
}

void LoopData::setWidth(int width)
{
    this->width = width;
}

void LoopData::setHeight(int height)
{
    this->height = height;
}


