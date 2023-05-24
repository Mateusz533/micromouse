
#ifndef MYROBOT_H
#define MYROBOT_H

#include "border.h"
#include "robot.h"
#include <vector>

class MyRobot : public Robot
{
  public:
    MyRobot() = delete;
    MyRobot(const int x, const int y);
    Movement run(const bool wall_left, const bool wall_right,
                 const bool wall_up, const bool wall_down) override;

  private:
    std::vector<std::vector<Border>> vertical_walls_;
    std::vector<std::vector<Border>> horizontal_walls_;
    int position_x_;
    int position_y_;
};

#endif  // MYROBOT_H
