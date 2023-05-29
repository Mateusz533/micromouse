
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
    void updateMaze(const Field &mouse_pose, const Field &target_pose,
                    const MazeSketch<bool, true> &sketch);
    void updateMousePosition(const Field &mouse_pose);

  protected:
    void paintEvent(QPaintEvent *event) override;

  private:
    void drawMaze(QPainter *painter);
    void drawCenteredImage(QPainter *painter, QImage &img, const Field &field,
                           const int frame_size);

    Field _mouse_position{ 0, 0 };
    Field _target_position{ 7, 7 };
    MazeSketch<bool, true> _sketch;
    QImage _mouse_img;
    QImage _target_img;
    QImage _prize_img;
};

#endif  // MAZEWIDGET_H
