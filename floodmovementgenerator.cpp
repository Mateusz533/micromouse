#include "floodmovementgenerator.h"
#include <set>

FloodMovementGenerator::FloodMovementGenerator(
  const int size, const Field &start, const Field &target,
  std::function<bool(const Movement &, const Field &)> isMovePossible)
  : isMovePossible{ isMovePossible }
  , _size{ size }
  , _target{ target }
  , _position{ start }
{
}

Movement FloodMovementGenerator::nextMove()
{
    if (_target == _position)
        return Movement::None;

    if (!_predicted_path.empty())
    {
        const Movement move{ _predicted_path.front() };
        if (isMovePossible(move, _position))
        {
            _predicted_path.pop();
            _position = _position.positionAfterMove(move);
            return move;
        }
    }

    if (!updatePath())
        return Movement::None;

    const Movement move{ _predicted_path.front() };
    _predicted_path.pop();
    _position = _position.positionAfterMove(move);

    return move;
}

Field FloodMovementGenerator::getPosition() const
{
    return _position;
}

bool FloodMovementGenerator::updatePath()
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

std::vector<std::vector<Movement>>
FloodMovementGenerator::getDijkstraPaths() const
{
    std::vector visited(_size, std::vector<bool>(_size, false));
    std::vector distances(_size, std::vector<unsigned int>(_size, -1));
    std::vector moves_from_pred(_size, std::vector(_size, Movement::None));
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
