
#include "myrobot.h"

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : position_x_(start_x)
  , position_y_(start_y)
  , target_x_(target_x)
  , target_y_(target_y)
{
    const auto column = std::vector<Border>(maze_size - 1, Border::Unknown);
    vertical_walls_ = std::vector<std::vector<Border>>(maze_size - 1, column);
    horizontal_walls_ = std::vector<std::vector<Border>>(maze_size - 1, column);
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    updateBorderWalls(wall_left, wall_right, wall_up, wall_down);
    return getFloodMovement();
}

void MyRobot::updateBorderWalls(const bool left, const bool right,
                                const bool up, const bool down)
{
    if (position_x_ < vertical_walls_.size())
        vertical_walls_[position_x_][position_y_] =
          right ? Border::Walled : Border::Empty;
    if (position_y_ < horizontal_walls_.size())
        horizontal_walls_[position_x_][position_y_] =
          down ? Border::Walled : Border::Empty;
    if (position_x_ > 0)
        vertical_walls_[position_x_ - 1][position_y_] =
          left ? Border::Walled : Border::Empty;
    if (position_y_ > 0)
        horizontal_walls_[position_x_][position_y_ - 1] =
          up ? Border::Walled : Border::Empty;
}

Movement MyRobot::getFloodMovement()
{
    return Movement::None;
}
