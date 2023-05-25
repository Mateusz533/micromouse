
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , maze_(16)
{
    ui->setupUi(this);
    timer_ = new QTimer(this);
    timer_->setInterval(1000);
    connect(timer_, &QTimer::timeout, this, [this]() { this->maze_.step(); });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer_;
}

void MainWindow::on_pbGenerate_clicked()
{
    timer_->stop();
    maze_.generateRandomSketch();
}

void MainWindow::on_pbLoad_clicked()
{
    timer_->stop();
    const std::string path =
      QFileDialog::getOpenFileName(this, tr("Open Maze"), "./",
                                   tr("Text Files (*.txt)"))
        .toStdString();
    const bool valid = maze_.getFromFile(path);
    if (!valid)
        ui->statusbar->showMessage(
          QString::fromUtf8("Nie można odczytać pliku."));
}

void MainWindow::on_pbStart_clicked()
{
    timer_->start();
}

void MainWindow::on_pbPause_clicked()
{
    timer_->stop();
}

void MainWindow::on_pbRestart_clicked()
{
    const int start_pose_x = ui->spbStartPoseX->value();
    const int start_pose_y = ui->spbStartPoseY->value();
    const int target_pose_x = ui->spbTargetPoseX->value();
    const int target_pose_y = ui->spbTargetPoseY->value();
    if (!maze_.setRobotPosition(start_pose_x, start_pose_y))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja startowa."));
        return;
    }
    if (!maze_.setTargetPosition(target_pose_x, target_pose_y))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja celu."));
        return;
    }
    maze_.restart();
    timer_->start();
}
