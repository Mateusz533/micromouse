#include "mazewidget.h"
#include <QPainter>
#include <QPainterPath>

MazeWidget::MazeWidget(QWidget *parent)
  : QWidget{ parent }
  , _sketch{ 16 }
{
    _mouse_img.load("../micromouse/img/mouse_scaled.png");
    _target_img.load("../micromouse/img/racing_flag_scaled.png");
    _prize_img.load("../micromouse/img/prize_scaled.png");
}

void MazeWidget::updateMaze(const Field &mouse_pose, const Field &target_pose,
                            const MazeSketch<bool, true> &sketch)
{
    _mouse_position = mouse_pose;
    _target_position = target_pose;
    _sketch = sketch;
    _path.clear();
    _path.push_back(mouse_pose);
    update();
}

void MazeWidget::updateMousePosition(const Field &mouse_pose)
{
    if (_mouse_position == mouse_pose)
        return;

    _path.push_back(mouse_pose);
    _mouse_position = mouse_pose;
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
    const int maze_size{ _sketch.size() };
    const int line_length{ static_cast<int>(
      std::round(1.0 * frame_size / maze_size)) };

    QPen pen(Qt::black, 4, Qt::SolidLine);
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
            if (_sketch.getWall(Field(i, j), Movement::Right))
                painter->drawLine(line_length * (1 + i), line_length * j,
                                  line_length * (1 + i), line_length * (j + 1));

            if (_sketch.getWall(Field(j, i), Movement::Down))
                painter->drawLine(line_length * j, line_length * (i + 1),
                                  line_length * (1 + j), line_length * (i + 1));
        }
    }

    drawPath(painter, line_length);
    drawItems(painter, line_length);
}

void MazeWidget::drawCenteredImage(QPainter *painter, QImage &img,
                                   const Field &field, const int frame_size)
{
    const int x = (frame_size - img.width()) / 2;
    const int y = (frame_size - img.height()) / 2;
    painter->drawImage(frame_size * field.x + x, frame_size * field.y + y, img);
}

void MazeWidget::drawItems(QPainter *painter, const int field_size)
{
    if (_target_position != _mouse_position)
    {
        drawCenteredImage(painter, _mouse_img, _mouse_position, field_size);
        drawCenteredImage(painter, _target_img, _target_position, field_size);
    }
    else
        drawCenteredImage(painter, _prize_img, _mouse_position, field_size);
}

void MazeWidget::drawPath(QPainter *painter, const int field_size)
{
    QPen pen(Qt::green, 4, Qt::SolidLine);
    painter->setPen(pen);

    for (int i = 0; i + 1 < _path.size(); ++i)
    {
        const QPoint start_point(field_size * (0.5 + _path[i].x),
                                 field_size * (0.5 + _path[i].y));
        const QPoint end_point(field_size * (0.5 + _path[i + 1].x),
                               field_size * (0.5 + _path[i + 1].y));
        painter->drawLine(start_point, end_point);
    }
}
