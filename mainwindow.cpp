#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = ui->playerLabel->getPlayer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video"),
                                                    ".",
                                                    tr("Video Files (*.avi *.mpg *.mp4")
                                                    );

    if (!filename.isEmpty())
    {
        this->player->loadVideo(filename.toStdString());
        this->player->play();
    }
}

void MainWindow::on_exportButton_clicked()
{
    auto loopData = this->ui->playerLabel->getPlayer()->getLoopData();
    if (loopData != nullptr)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save video file"), QString::fromUtf8((loopData->filename + "_roi").c_str()),
                tr("Videos (*.mp4)"));

        this->ui->playerLabel->writeFile(fileName.toStdString());
    }

}

