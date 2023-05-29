
#include "myrobot.h"
#include <algorithm>
#include <set>

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : target_{ target_x, target_y }
  , position_{ start_x, start_y }
  , predicted_sketch_{ maze_size, Border::Unknown }
{
}
MyRobot::MyRobot(const Field &start, const Field &target, const int maze_size)
  : target_{ target }
  , position_{ start }
  , predicted_sketch_{ maze_size, Border::Unknown }
{
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    updateBorderWalls(wall_left, wall_right, wall_up, wall_down);
    const Movement move{ getFloodMovement() };
    position_ = position_.positionAfterMove(move);
    return move;
}

void MyRobot::updateBorderWalls(const bool left, const bool right,
                                const bool up, const bool down)
{
    predicted_sketch_.setWall(position_, Movement::Left,
                              left ? Border::Walled : Border::Empty);
    predicted_sketch_.setWall(position_, Movement::Right,
                              right ? Border::Walled : Border::Empty);
    predicted_sketch_.setWall(position_, Movement::Up,
                              up ? Border::Walled : Border::Empty);
    predicted_sketch_.setWall(position_, Movement::Down,
                              down ? Border::Walled : Border::Empty);
}

Movement MyRobot::getFloodMovement()
{
    if (target_ == position_)
        return Movement::None;

    if (!predicted_path_.empty())
    {
        const Movement move{ predicted_path_.front() };
        if (isMovePossible(move, position_))
        {
            predicted_path_.pop();
            return move;
        }
    }

    if (!updatePath())
        return Movement::None;

    const Movement move{ predicted_path_.front() };
    predicted_path_.pop();
    return move;
}

std::vector<std::vector<Movement>> MyRobot::getDijkstraPaths() const
{
    const int size{ predicted_sketch_.size() };
    std::vector visited(size, std::vector<bool>(size, false));
    std::vector distances(size, std::vector<unsigned int>(size, -1));
    std::vector movesFromPred(size, std::vector(size, Movement::None));
    std::set<Field> fields;
    Field current_field{ position_ };
    distances[position_.x][position_.y] = 0;
    fields.insert(current_field);

    while (!fields.empty() && current_field != target_)
    {
        const auto &min_itr = std::min_element(
          fields.begin(), fields.end(), [&distances](auto &left, auto &right) {
              return distances[left.x][left.y] < distances[right.x][right.y];
          });
        current_field = *min_itr;
        fields.erase(min_itr);
        const auto &min_distance{ distances[current_field.x][current_field.y] };

        std::vector<Movement> moves_range{ Movement::Left, Movement::Right,
                                           Movement::Up, Movement::Down };

        for (const auto &move : moves_range)
        {
            if (!isMovePossible(move, current_field))
                continue;

            const Field neighbor{ current_field.positionAfterMove(move) };

            if (!visited[neighbor.x][neighbor.y])
            {
                fields.insert(neighbor);
                visited[neighbor.x][neighbor.y] = true;
            }

            if (distances[neighbor.x][neighbor.y] - 1 <= min_distance)
                continue;

            distances[neighbor.x][neighbor.y] = min_distance + 1;
            movesFromPred[neighbor.x][neighbor.y] = move;
        }
    }

    return movesFromPred;
}

bool MyRobot::updatePath()
{
    const auto path_map{ getDijkstraPaths() };
    std::deque<Movement> new_path;
    Field current_position{ target_ };

    while (current_position != position_)
    {
        const Movement move{ path_map[current_position.x][current_position.y] };
        if (move == Movement::None)
            return false;

        current_position = current_position.positionBeforeMove(move);
        new_path.push_front(move);
    }

    predicted_path_ = std::queue<Movement>(new_path);
    return true;
}

bool MyRobot::isMovePossible(const Movement move, const Field &from) const
{
    return predicted_sketch_.getWall(from, move) != Border::Walled;
}
