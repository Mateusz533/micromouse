
#ifndef MYROBOT_H
#define MYROBOT_H

#include "field.h"
#include "mazesketch.h"
#include "robot.h"
#include <queue>
#include <vector>

/** \brief Przykład robota 'micromouse'
 *
 *  Autorska klasa implementująca interfejs robota 'micromouse'.
 */
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
    enum class Border
    {
        Empty,
        Walled,
        Unknown,
    };

    void updateBorderWalls(const bool left, const bool right, const bool up,
                           const bool down);
    Movement getFloodMovement();
    bool updatePath();
    std::vector<std::vector<Movement>> getDijkstraPaths() const;
    bool isMovePossible(const Movement move, const Field &from) const;

    const Field _target;
    Field _position;
    MazeSketch<Border, Border::Walled> _predicted_sketch;
    std::queue<Movement> _predicted_path;
};

#endif  // MYROBOT_H
