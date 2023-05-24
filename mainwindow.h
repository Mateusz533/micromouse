
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

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

    void on_pbRestart_clicked();

private:
    Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
