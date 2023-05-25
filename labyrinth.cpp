
#include "labyrinth.h"
#include "myrobot.h"

Labyrinth::Labyrinth(const int size)
  : mouse_(new MyRobot(0, 0, 7, 7, 16))
{
    const std::vector<bool> column(size - 1, false);
    vertical_walls_ = std::vector<std::vector<bool>>(size - 1, column);
    horizontal_walls_ = std::vector<std::vector<bool>>(size - 1, column);
}

bool Labyrinth::getSketchFromFile(const std::string &path)
{
    // TODO
    return false;
}

void Labyrinth::generateRandomSketch()
{
    // TODO
}

void Labyrinth::restart()
{
    const int size = vertical_walls_.size();
    mouse_.reset(new MyRobot(mouse_x_, mouse_y_, target_x_, target_y_, size));
}

bool Labyrinth::setTargetPosition(const int x, const int y)
{
    const int size = vertical_walls_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    target_x_ = x;
    target_y_ = y;
    return true;
}

bool Labyrinth::setRobotPosition(const int x, const int y)
{
    const int size = vertical_walls_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    mouse_x_ = x;
    mouse_y_ = y;
    return true;
}

std::pair<int, int> Labyrinth::getTargetPosition() const
{
    return std::pair<int, int>(target_x_, target_y_);
}

std::pair<int, int> Labyrinth::getRobotPosition() const
{
    return std::pair<int, int>(mouse_x_, mouse_y_);
}

std::vector<std::vector<std::pair<bool, bool>>> Labyrinth::getSketch() const
{
    std::vector<std::vector<std::pair<bool, bool>>> result;
    const int size = vertical_walls_.size();
    for (int i = 0; i < size; ++i)
    {
        std::vector<std::pair<bool, bool>> column;
        for (int j = 0; j < size; ++j)
        {
            const std::pair<bool, bool> walls(vertical_walls_[i][j],
                                              horizontal_walls_[i][j]);
            column.push_back(walls);
        }
        result.push_back(column);
    }
    return result;
}

void Labyrinth::step()
{
    const int size = vertical_walls_.size();
    const bool left =
      mouse_x_ > 0 ? vertical_walls_[mouse_x_ - 1][mouse_y_] : true;
    const bool right =
      mouse_x_ < size ? vertical_walls_[mouse_x_][mouse_y_] : true;
    const bool up =
      mouse_y_ > 0 ? horizontal_walls_[mouse_x_][mouse_y_ - 1] : true;
    const bool down =
      mouse_y_ < size ? horizontal_walls_[mouse_x_][mouse_y_] : true;
    const Movement movement = mouse_->run(left, right, up, down);
    moveMouse(movement);
}

void Labyrinth::moveMouse(Movement movement)
{
    switch (movement)
    {
    case Movement::Left:

        break;
    case Movement::Right:

        break;
    case Movement::Up:

        break;
    case Movement::Down:

        break;
    case Movement::None:

        break;
    }
}
