
#ifndef ROBOT_H
#define ROBOT_H

#include "movement.h"

class Robot
{
  public:
    Robot() = delete;
    virtual Movement run(const bool wall_left, const bool wall_right,
                         const bool wall_up, const bool wall_down) = 0;
};

#endif  // ROBOT_H
