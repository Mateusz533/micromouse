#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "labyrinth.h"
#include <QMainWindow>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

/** \brief Okno graficzne 'micromouse'
 *
 *  Klasa tworząca interfejs graficzny do symulacji zawodów 'micromouse'.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_pbGenerate_clicked();

    void on_pbLoad_clicked();

    void on_pbStart_clicked();

    void on_pbPause_clicked();

    void on_pbReset_clicked();

  private:
    void drawMouseMove();
    void reset();

    Ui::MainWindow *ui;
    QTimer *p_timer;
    Labyrinth _maze_puzzle;
};

#endif  // MAINWINDOW_H
