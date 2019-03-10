#ifndef LOOPPLAYERLABEL_H
#define LOOPPLAYERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "loopplayer.h"
#include <QBitmap>
#include <vector>
#include <QVector>

#include "looproi.h"

class LoopPlayerLabel : public QLabel
{
    Q_OBJECT
public:
    explicit LoopPlayerLabel(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void resizeEvent(QResizeEvent *event);

    LoopPlayer * getPlayer();

    void writeFile(std::string filepath);

private:
    LoopPlayer * player;
    bool pressing = false;
    QVector<QPoint> * selectedRoi;
    QVector<QPoint> * displayingRoi;

    int contentWidth = 0;
    int contentHeight = 0;
    double contentRatio = 0.00;
    int screenWidth = 0;
    int screenHeight = 0;
    double screenRatio = 0.00;

    double contentScale = 1.00;

    int edgeLeft = 0;
    int edgeTop = 0;
    int edgeRight = 0;
    int edgeBottom = 0;

    void updateLayoutValue();

    LoopRoi loopRoi;

signals:

public slots:
    void updateLoopPlayerUI(QImage img);
};

#endif // LOOPPLAYERLABEL_H
