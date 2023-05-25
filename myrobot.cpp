
#include "myrobot.h"

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : predicted_sketch_(maze_size, Border::Unknown)
  , position_x_(start_x)
  , position_y_(start_y)
  , target_x_(target_x)
  , target_y_(target_y)
{
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
    predicted_sketch_.setVerticalWall(position_x_, position_y_,
                                      right ? Border::Walled : Border::Empty);
    predicted_sketch_.setVerticalWall(position_x_ - 1, position_y_,
                                      left ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_x_, position_y_,
                                        down ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_x_, position_y_ - 1,
                                        up ? Border::Walled : Border::Empty);
}

Movement MyRobot::getFloodMovement()
{
    return Movement::None;
}
