
#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "mazesketch.h"
#include <QWidget>

class MazeWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void updateMaze(std::pair<int, int> mouse_pose,
                    std::pair<int, int> target_pose,
                    MazeSketch<bool, true> sketch);
    void updateMousePosition(std::pair<int, int> mouse_pose);

  protected:
    void paintEvent(QPaintEvent *event) override;
    void drawMaze(QPainter *painter);

  private:
    std::pair<int, int> mouse_pose_{ std::pair<int, int>(0, 0) };
    std::pair<int, int> target_pose_{ std::pair<int, int>(7, 7) };
    MazeSketch<bool, true> sketch_;
};

#endif  // MAZEWIDGET_H
