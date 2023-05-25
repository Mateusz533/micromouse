
#ifndef MYROBOT_H
#define MYROBOT_H

#include "border.h"
#include "robot.h"
#include <vector>

class MyRobot : public Robot
{
  public:
    MyRobot() = delete;
    MyRobot(const int start_x, const int start_y, const int target_x,
            const int target_y, const int maze_size);
    Movement run(const bool wall_left, const bool wall_right,
                 const bool wall_up, const bool wall_down) override;

  private:
    void updateBorderWalls(const bool left, const bool right, const bool up,
                           const bool down);
    Movement getFloodMovement();

    std::vector<std::vector<Border>> vertical_walls_;
    std::vector<std::vector<Border>> horizontal_walls_;
    int position_x_;
    int position_y_;
    const int target_x_;
    const int target_y_;
};

#endif  // MYROBOT_H
