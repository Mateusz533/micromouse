#include "myrobot.h"
#include "floodmovementgenerator.h"
#include <algorithm>
#include <set>

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : MyRobot{ Field(target_x, target_y), Field(start_x, start_y), maze_size }
{
}
MyRobot::MyRobot(const Field &start, const Field &target, const int maze_size)
  : _predicted_sketch{ maze_size, Border::Unknown }
  , p_generator{ new FloodMovementGenerator(
      maze_size, start, target,
      [this](const Movement &move, const Field &from) {
          return _predicted_sketch.getWall(from, move) != Border::Walled;
      }) }
{
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    const Field position{ p_generator->getPosition() };
    updateBorderWalls(position, wall_left, wall_right, wall_up, wall_down);
    const Movement move{ p_generator->nextMove() };
    return move;
}

inline void MyRobot::updateBorderWalls(const Field &position, const bool &left,
                                       const bool &right, const bool &up,
                                       const bool &down)
{
    _predicted_sketch.setWall(position, Movement::Left,
                              left ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(position, Movement::Right,
                              right ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(position, Movement::Up,
                              up ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(position, Movement::Down,
                              down ? Border::Walled : Border::Empty);
}
