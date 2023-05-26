
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QTimer>

const int STEP_TIME_INTERVAL_MS = 200;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , maze_puzzle_(16)
{
    ui->setupUi(this);

    timer_ = new QTimer(this);
    timer_->setInterval(STEP_TIME_INTERVAL_MS);
    connect(timer_, &QTimer::timeout, this, [this]() {
        this->maze_puzzle_.step();
        const auto mouse_pose = maze_puzzle_.getRobotPosition();
        drawMouseMove(mouse_pose);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer_;
}

void MainWindow::on_pbGenerate_clicked()
{
    timer_->stop();
    maze_puzzle_.generateRandomSketch();
    reset();
}

void MainWindow::on_pbLoad_clicked()
{
    timer_->stop();
    const std::string path =
      QFileDialog::getOpenFileName(this, tr("Open Maze"), "./",
                                   tr("Text Files (*.txt)"))
        .toStdString();
    const bool valid = maze_puzzle_.getSketchFromFile(path);
    if (!valid)
        ui->statusbar->showMessage(
          QString::fromUtf8("Nie można odczytać pliku."));
    else
        reset();
}

void MainWindow::on_pbStart_clicked()
{
    timer_->start();
}

void MainWindow::on_pbPause_clicked()
{
    timer_->stop();
}

void MainWindow::on_pbReset_clicked()
{
    timer_->stop();
    reset();
}

void MainWindow::drawMouseMove(std::pair<int, int>)
{
    const std::pair<int, int> mouse_pose = maze_puzzle_.getRobotPosition();
    ui->wdgMaze->updateMousePosition(mouse_pose);
}

void MainWindow::reset()
{
    const int start_pose_x = ui->spbStartPoseX->value();
    const int start_pose_y = ui->spbStartPoseY->value();
    const int target_pose_x = ui->spbTargetPoseX->value();
    const int target_pose_y = ui->spbTargetPoseY->value();
    if (!maze_puzzle_.setRobotPosition(start_pose_x, start_pose_y))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja startowa."));
        return;
    }
    if (!maze_puzzle_.setTargetPosition(target_pose_x, target_pose_y))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja celu."));
        return;
    }
    maze_puzzle_.restart();
    const std::pair<int, int> target_pose = maze_puzzle_.getTargetPosition();
    const std::pair<int, int> mouse_pose = maze_puzzle_.getRobotPosition();
    const MazeSketch sketch = maze_puzzle_.getSketch();
    ui->wdgMaze->updateMaze(mouse_pose, target_pose, sketch);
}
