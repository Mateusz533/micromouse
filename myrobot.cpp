
#include "myrobot.h"
#include <algorithm>
#include <set>

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : _target{ target_x, target_y }
  , _position{ start_x, start_y }
  , _predicted_sketch{ maze_size, Border::Unknown }
{
}
MyRobot::MyRobot(const Field &start, const Field &target, const int maze_size)
  : _target{ target }
  , _position{ start }
  , _predicted_sketch{ maze_size, Border::Unknown }
{
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    updateBorderWalls(wall_left, wall_right, wall_up, wall_down);
    const Movement move{ getFloodMovement() };
    _position = _position.positionAfterMove(move);
    return move;
}

void MyRobot::updateBorderWalls(const bool left, const bool right,
                                const bool up, const bool down)
{
    _predicted_sketch.setWall(_position, Movement::Left,
                              left ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(_position, Movement::Right,
                              right ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(_position, Movement::Up,
                              up ? Border::Walled : Border::Empty);
    _predicted_sketch.setWall(_position, Movement::Down,
                              down ? Border::Walled : Border::Empty);
}

Movement MyRobot::getFloodMovement()
{
    if (_target == _position)
        return Movement::None;

    if (!_predicted_path.empty())
    {
        const Movement move{ _predicted_path.front() };
        if (isMovePossible(move, _position))
        {
            _predicted_path.pop();
            return move;
        }
    }

    if (!updatePath())
        return Movement::None;

    const Movement move{ _predicted_path.front() };
    _predicted_path.pop();
    return move;
}

bool MyRobot::updatePath()
{
    const auto path_map{ getDijkstraPaths() };
    std::deque<Movement> new_path;
    Field current_position{ _target };

    while (current_position != _position)
    {
        const Movement move{ path_map[current_position.x][current_position.y] };
        if (move == Movement::None)
            return false;

        current_position = current_position.positionBeforeMove(move);
        new_path.push_front(move);
    }

    _predicted_path = std::queue<Movement>(new_path);
    return true;
}

std::vector<std::vector<Movement>> MyRobot::getDijkstraPaths() const
{
    const int size{ _predicted_sketch.size() };
    std::vector visited(size, std::vector<bool>(size, false));
    std::vector distances(size, std::vector<unsigned int>(size, -1));
    std::vector moves_from_pred(size, std::vector(size, Movement::None));
    std::set<Field> fields_to_check;
    Field min_field{ _position };
    distances[_position.x][_position.y] = 0;
    fields_to_check.insert(min_field);

    while (!fields_to_check.empty() && min_field != _target)
    {
        const auto &min_itr = std::min_element(
          fields_to_check.begin(), fields_to_check.end(),
          [&distances](auto &left, auto &right) {
              return distances[left.x][left.y] < distances[right.x][right.y];
          });
        min_field = *min_itr;
        fields_to_check.erase(min_itr);
        const auto &min_distance{ distances[min_field.x][min_field.y] };

        std::vector<Movement> moves_range{ Movement::Left, Movement::Right,
                                           Movement::Up, Movement::Down };

        for (const auto &move : moves_range)
        {
            if (!isMovePossible(move, min_field))
                continue;

            const Field neighbor{ min_field.positionAfterMove(move) };

            if (!visited[neighbor.x][neighbor.y])
            {
                fields_to_check.insert(neighbor);
                visited[neighbor.x][neighbor.y] = true;
            }

            if (distances[neighbor.x][neighbor.y] - 1 <= min_distance)
                continue;

            distances[neighbor.x][neighbor.y] = min_distance + 1;
            moves_from_pred[neighbor.x][neighbor.y] = move;
        }
    }

    return moves_from_pred;
}

bool MyRobot::isMovePossible(const Movement move, const Field &from) const
{
    return _predicted_sketch.getWall(from, move) != Border::Walled;
}
