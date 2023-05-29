
#include "mazewidget.h"
#include <QPainter>
#include <QPainterPath>

MazeWidget::MazeWidget(QWidget *parent)
  : QWidget{ parent }
  , sketch_{ 16 }
{
    mouse_img_.load("../micromouse/img/mouse_scaled.png");
    target_img_.load("../micromouse/img/racing_flag_scaled.png");
    prize_img_.load("../micromouse/img/prize_scaled.png");
}

void MazeWidget::updateMaze(const Field &mouse_pose, const Field &target_pose,
                            const MazeSketch<bool, true> &sketch)
{
    mouse_pose_ = mouse_pose;
    target_pose_ = target_pose;
    sketch_ = sketch;
    update();
}

void MazeWidget::updateMousePosition(const Field &mouse_pose)
{
    mouse_pose_ = mouse_pose;
    update();
}

void MazeWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    drawMaze(&painter);
}

void MazeWidget::drawMaze(QPainter *painter)
{
    const int frame_size{ this->size().height() };
    const int maze_size{ sketch_.size() };
    const int line_length{ static_cast<int>(
      std::round(1.0 * frame_size / maze_size)) };

    QPen pen(Qt::black, 3, Qt::SolidLine);
    painter->setPen(pen);
    QBrush background = QBrush(QColor(255, 255, 255));
    painter->fillRect(0, 0, frame_size, frame_size, background);
    painter->drawRect(1, 1, frame_size - 2, frame_size - 2);
    pen = QPen(Qt::black, 2, Qt::SolidLine);
    painter->setPen(pen);

    for (int i = 0; i < maze_size - 1; ++i)
    {
        for (int j = 0; j < maze_size; ++j)
        {
            if (sketch_.getVerticalWall(i, j))
                painter->drawLine(line_length * (1 + i), line_length * j,
                                  line_length * (1 + i), line_length * (j + 1));

            if (sketch_.getHorizontalWall(j, i))
                painter->drawLine(line_length * j, line_length * (i + 1),
                                  line_length * (1 + j), line_length * (i + 1));
        }
    }

    if (target_pose_ != mouse_pose_)
    {
        drawCenteredImage(painter, mouse_img_, mouse_pose_, line_length);
        drawCenteredImage(painter, target_img_, target_pose_, line_length);
    }
    else
        drawCenteredImage(painter, prize_img_, mouse_pose_, line_length);
}

void MazeWidget::drawCenteredImage(QPainter *painter, QImage &img,
                                   const Field &field, const int frame_size)
{
    const int x = (frame_size - img.width()) / 2;
    const int y = (frame_size - img.height()) / 2;
    painter->drawImage(frame_size * field.x + x, frame_size * field.y + y, img);
}
