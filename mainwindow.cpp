
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbGenerate_clicked()
{

}


void MainWindow::on_pbLoad_clicked()
{

}


void MainWindow::on_pbStart_clicked()
{

}


void MainWindow::on_pbPause_clicked()
{

}


void MainWindow::on_pbRestart_clicked()
{

}

