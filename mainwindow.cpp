
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QTimer>

const int STEP_TIME_INTERVAL_MS = 200;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{ parent }
  , ui{ new Ui::MainWindow }
  , _maze_puzzle{ 16 }
{
    ui->setupUi(this);

    p_timer = new QTimer(this);
    p_timer->setInterval(STEP_TIME_INTERVAL_MS);
    connect(p_timer, &QTimer::timeout, this, [this]() {
        this->_maze_puzzle.step();
        drawMouseMove();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete p_timer;
}

void MainWindow::on_pbGenerate_clicked()
{
    p_timer->stop();
    _maze_puzzle.generateRandomSketch();
    reset();
}

void MainWindow::on_pbLoad_clicked()
{
    p_timer->stop();
    const std::string path{ QFileDialog::getOpenFileName(
                              this, tr("Open Maze"), "./",
                              tr("Text Files (*.txt)"))
                              .toStdString() };
    const bool valid{ _maze_puzzle.getSketchFromFile(path) };

    if (!valid)
        ui->statusbar->showMessage(
          QString::fromUtf8("Nie można odczytać pliku."));
    else
        reset();
}

void MainWindow::on_pbStart_clicked()
{
    p_timer->start();
}

void MainWindow::on_pbPause_clicked()
{
    p_timer->stop();
}

void MainWindow::on_pbReset_clicked()
{
    p_timer->stop();
    reset();
}

void MainWindow::drawMouseMove()
{
    const Field mouse_position{ _maze_puzzle.getRobotPosition() };
    ui->wdgMaze->updateMousePosition(mouse_position);
}

void MainWindow::reset()
{
    const Field mouse_position{ ui->spbStartPoseX->value(),
                                ui->spbStartPoseY->value() };
    const Field target_position{ ui->spbTargetPoseX->value(),
                                 ui->spbTargetPoseY->value() };

    if (!_maze_puzzle.setRobotPosition(mouse_position))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja startowa."));
        return;
    }
    if (!_maze_puzzle.setTargetPosition(target_position))
    {
        ui->statusbar->showMessage(
          QString::fromUtf8("Nieprawidłowa pozycja celu."));
        return;
    }

    ui->statusbar->clearMessage();

    _maze_puzzle.restart();
    const MazeSketch sketch{ _maze_puzzle.getSketch() };
    ui->wdgMaze->updateMaze(mouse_position, target_position, sketch);
}
