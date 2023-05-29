
#ifndef MYROBOT_H
#define MYROBOT_H

#include "border.h"
#include "field.h"
#include "mazesketch.h"
#include "robot.h"
#include <queue>
#include <vector>

class MyRobot : public Robot
{
  public:
    MyRobot() = delete;
    MyRobot(const int start_x, const int start_y, const int target_x,
            const int target_y, const int maze_size);
    MyRobot(const Field &start, const Field &target, const int maze_size);
    Movement run(const bool wall_left, const bool wall_right,
                 const bool wall_up, const bool wall_down) override;

  private:
    void updateBorderWalls(const bool left, const bool right, const bool up,
                           const bool down);
    Movement getFloodMovement();
    std::vector<std::vector<Movement>> getDijkstraPaths() const;
    bool updatePath();
    bool isMovePossible(const Movement move, const Field &from) const;

    const Field target_;
    Field position_;
    MazeSketch<Border, Border::Walled> predicted_sketch_;
    std::queue<Movement> predicted_path_;
};

#endif  // MYROBOT_H
