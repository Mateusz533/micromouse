
#include "mazewidget.h"
#include <QPainter>
#include <QPainterPath>

MazeWidget::MazeWidget(QWidget *parent)
  : QWidget{ parent }
  , sketch_{ 16 }
{
}

void MazeWidget::updateMaze(std::pair<int, int> mouse_pose,
                            std::pair<int, int> target_pose,
                            MazeSketch<bool, true> sketch)
{
    mouse_pose_ = mouse_pose;
    target_pose_ = target_pose;
    sketch_ = sketch;
    update();
}

void MazeWidget::updateMousePosition(std::pair<int, int> mouse_pose)
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
    const int frame_size = 480;
    const int maze_size = sketch_.size();
    const double line_length = 1.0 * frame_size / maze_size;

    QPen pen(Qt::black, 1, Qt::SolidLine);
    painter->setPen(pen);
    QBrush background = QBrush(QColor(255, 255, 255));
    painter->fillRect(0, 1, frame_size, frame_size - 1, background);
    painter->drawRect(0, 1, frame_size, frame_size - 1);

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

    QPointF mouse_pose = QPointF(line_length * (mouse_pose_.first + 0.5),
                                 line_length * (mouse_pose_.second + 0.5));
    const double mouse_r = line_length / 4;
    QPainterPath mouse_path;
    mouse_path.addEllipse(mouse_pose, mouse_r, mouse_r);
    painter->fillPath(mouse_path, QBrush(Qt::green));

    QPoint target_pose = QPoint(line_length * (target_pose_.first + 0.5),
                                line_length * (target_pose_.second + 0.5));
    const double target_r = line_length / 4;
    QPainterPath target_path;
    target_path.addEllipse(target_pose, target_r, target_r);
    painter->fillPath(target_path, QBrush(Qt::red));
}
