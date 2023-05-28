
#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include "field.h"
#include "mazesketch.h"
#include <QWidget>

class MazeWidget : public QWidget
{
    Q_OBJECT
  public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void updateMaze(Field mouse_pose, Field target_pose,
                    MazeSketch<bool, true> sketch);
    void updateMousePosition(Field mouse_pose);

  protected:
    void paintEvent(QPaintEvent *event) override;

  private:
    void drawMaze(QPainter *painter);
    void drawCenteredImage(QPainter *painter, QImage &img, Field field,
                           const int frame_size);

    Field mouse_pose_{ 0, 0 };
    Field target_pose_{ 7, 7 };
    MazeSketch<bool, true> sketch_;
    QImage mouse_img_;
    QImage target_img_;
    QImage prize_img_;
};

#endif  // MAZEWIDGET_H
