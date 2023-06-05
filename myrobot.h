#ifndef MYROBOT_H
#define MYROBOT_H

#include "field.h"
#include "mazesketch.h"
#include "movementgenerator.h"
#include "robot.h"
#include <memory>
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

    inline void updateBorderWalls(const Field &position, const bool &left,
                                  const bool &right, const bool &up,
                                  const bool &down);

    MazeSketch<Border, Border::Walled> _predicted_sketch;
    std::unique_ptr<MovementGenerator> p_generator;
};

#endif  // MYROBOT_H
