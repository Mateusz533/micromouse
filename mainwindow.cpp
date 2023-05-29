
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QTimer>

const int STEP_TIME_INTERVAL_MS = 200;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{ parent }
  , ui{ new Ui::MainWindow }
  , maze_puzzle_{ 16 }
{
    ui->setupUi(this);

    timer_ = new QTimer(this);
    timer_->setInterval(STEP_TIME_INTERVAL_MS);
    connect(timer_, &QTimer::timeout, this, [this]() {
        this->maze_puzzle_.step();
        drawMouseMove();
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
    const std::string path{ QFileDialog::getOpenFileName(
                              this, tr("Open Maze"), "./",
                              tr("Text Files (*.txt)"))
                              .toStdString() };
    const bool valid{ maze_puzzle_.getSketchFromFile(path) };

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

void MainWindow::drawMouseMove()
{
    const Field mouse_position{ maze_puzzle_.getRobotPosition() };
    ui->wdgMaze->updateMousePosition(mouse_position);
}

void MainWindow::reset()
{
    const Field mouse_position{ ui->spbStartPoseX->value(),
                                ui->spbStartPoseY->value() };
    const Field target_position{ ui->spbTargetPoseX->value(),
                                 ui->spbTargetPoseY->value() };

    if (!maze_puzzle_.setRobotPosition(mouse_position))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja startowa."));
        return;
    }
    if (!maze_puzzle_.setTargetPosition(target_position))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja celu."));
        return;
    }

    ui->statusbar->clearMessage();

    maze_puzzle_.restart();
    const MazeSketch sketch{ maze_puzzle_.getSketch() };
    ui->wdgMaze->updateMaze(mouse_position, target_position, sketch);
}
