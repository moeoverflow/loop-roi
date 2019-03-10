#include "loopplayerlabel.h"
#include <QPainter>

LoopPlayerLabel::LoopPlayerLabel(QWidget *parent) : QLabel(parent)
{

    this->player = new LoopPlayer();
    this->displayingRoi = new QVector<QPoint>();
    this->selectedRoi = new QVector<QPoint>();

    QObject::connect(this->player, SIGNAL(processedImage(QImage)), this, SLOT(updateLoopPlayerUI(QImage)));

    this->updateLayoutValue();
}

LoopPlayer * LoopPlayerLabel::getPlayer() {
    return this->player;
}

void LoopPlayerLabel::updateLoopPlayerUI(QImage img)
{

    if (!img.isNull())
    {
        this->setAlignment(Qt::AlignCenter);
        QPixmap p = QPixmap::fromImage(img);
        QPainter painter(&p);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen(QColor(0, 0, 255), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLines(*this->loopRoi.points);
        if (!this->loopRoi.drawing) {
            auto prevPoint = *this->loopRoi.points->begin();
            auto nextPoint = *(this->loopRoi.points->end()-1);
            painter.drawLine(prevPoint, nextPoint);
        }

        this->setPixmap(p.scaled(this->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
}

void LoopPlayerLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (this->loopRoi.drawing)
    {
        int x = event->x();
        int y = event->y();
        if (x < edgeLeft) x = edgeLeft;
        if (x > edgeRight) x = edgeRight;
        if (y < edgeTop) y = edgeTop;
        if (y > edgeBottom) y = edgeBottom;
        x = static_cast<int>((x-edgeLeft) / contentScale);
        y = static_cast<int>((y-edgeTop) / contentScale);

        QPoint point(x, y);
        this->loopRoi.points->append(point);
    }
}

void LoopPlayerLabel::mousePressEvent(QMouseEvent *event)
{
    auto loopData = this->player->getLoopData();
    if (loopData == nullptr) return;

    this->loopRoi.drawing = true;
    this->loopRoi.points = new QVector<QPoint>();
    this->loopRoi.width = this->width();
    this->loopRoi.height = this->height();
    this->updateLayoutValue();
}

void LoopPlayerLabel::mouseReleaseEvent(QMouseEvent *event)
{
    auto player = this->getPlayer();
    auto loopData = player->getLoopData();
    if (loopData == nullptr) return;

    this->loopRoi.drawing = false;

    player->pause();
    loopData->loadData();
    auto frames = loopData->getFrames();
    auto roiFrames = this->loopRoi.generateRoiFrames(frames);
    loopData->setFrames(roiFrames);
    player->play();

}

void LoopPlayerLabel::resizeEvent(QResizeEvent *event)
{
    this->updateLayoutValue();
}

void LoopPlayerLabel::updateLayoutValue()
{
    auto loopData = this->player->getLoopData();

    if (loopData != nullptr)
    {
        contentWidth = loopData->getWidth();
        contentHeight = loopData->getHeight();
    }

    contentRatio = static_cast<double>(contentWidth) / static_cast<double>(contentHeight);
    screenWidth = this->width();
    screenHeight = this->height();
    screenRatio = static_cast<double>(screenWidth) / static_cast<double>(screenHeight);


    edgeLeft = 0;
    edgeRight = screenWidth;
    edgeTop = 0;
    edgeBottom = screenHeight;

    if (contentRatio > screenRatio)
    {
        contentScale = static_cast<double>(screenWidth) / static_cast<double>(contentWidth);
        int edge = static_cast<int>((screenHeight - contentHeight * contentScale) / 2);
        edgeTop += edge;
        edgeBottom -= edge;
    } else if (contentRatio < screenRatio)
    {
        contentScale = static_cast<double>(screenHeight) / static_cast<double>(contentHeight);
        int edge = static_cast<int>((screenWidth - contentWidth * contentScale) / 2);
        edgeLeft += edge;
        edgeRight -= edge;
    }
}

