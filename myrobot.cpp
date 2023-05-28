
#include "myrobot.h"
#include <algorithm>
#include <set>
#include <stack>

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : predicted_sketch_(maze_size, Border::Unknown)
  , position_(start_x, start_y)
  , target_(target_x, target_y)
{
}
MyRobot::MyRobot(const Field start, const Field target, const int maze_size)
  : predicted_sketch_(maze_size, Border::Unknown)
  , position_(start)
  , target_(target)
{
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    updateBorderWalls(wall_left, wall_right, wall_up, wall_down);
    const Movement move = getFloodMovement();
    position_ = position_.positionAfterMove(move);
    return move;
}

void MyRobot::updateBorderWalls(const bool left, const bool right,
                                const bool up, const bool down)
{
    predicted_sketch_.setVerticalWall(position_.x, position_.y,
                                      right ? Border::Walled : Border::Empty);
    predicted_sketch_.setVerticalWall(position_.x - 1, position_.y,
                                      left ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_.x, position_.y,
                                        down ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_.x, position_.y - 1,
                                        up ? Border::Walled : Border::Empty);
}

Movement MyRobot::getFloodMovement()
{
    if (target_ == position_)
        return Movement::None;

    if (!predicted_path_.empty())
    {
        const auto move = predicted_path_.front();
        if (isMovePossible(move, position_))
        {
            predicted_path_.pop();
            return move;
        }
    }

    if (!updatePath())
        return Movement::None;

    const auto move = predicted_path_.front();
    predicted_path_.pop();
    return move;
}

std::vector<std::vector<Movement>> MyRobot::getDijkstraPaths() const
{
    const int size = predicted_sketch_.size();
    std::vector visited(size, std::vector<bool>(size, false));
    std::vector distances(size, std::vector<unsigned int>(size, -1));
    std::vector movesFromPred(size, std::vector(size, Movement::None));
    std::set<Field> fields;
    Field current_field{ position_ };
    distances[position_.x][position_.y] = 0;
    fields.insert(current_field);

    while (!fields.empty() && current_field != target_)
    {
        const auto itr = std::min_element(
          fields.begin(), fields.end(), [&distances](auto &left, auto &right) {
              return distances[left.x][left.y] < distances[right.x][right.y];
          });
        current_field = *itr;
        fields.erase(itr);
        const auto &min_x = current_field.x;
        const auto &min_y = current_field.y;

        std::vector<Movement> moves_range(
          { Movement::Left, Movement::Right, Movement::Up, Movement::Down });

        for (const auto &move : moves_range)
        {
            if (!isMovePossible(move, current_field))
                continue;

            const Field neighbor = current_field.positionAfterMove(move);

            if (!visited[neighbor.x][neighbor.y])
            {
                fields.insert(neighbor);
                visited[neighbor.x][neighbor.y] = true;
            }

            if (distances[neighbor.x][neighbor.y] - 1 <=
                distances[min_x][min_y])
                continue;

            distances[neighbor.x][neighbor.y] = distances[min_x][min_y] + 1;
            movesFromPred[neighbor.x][neighbor.y] = move;
        }
    }

    return movesFromPred;
}

bool MyRobot::updatePath()
{
    const auto path_map = getDijkstraPaths();
    std::stack<Movement> path_buffer;
    Field current_position{ target_ };

    while (current_position != position_)
    {
        const auto move = path_map[current_position.x][current_position.y];
        if (move == Movement::None)
            return false;

        current_position = current_position.positionBeforeMove(move);
        path_buffer.push(move);
    }

    std::queue<Movement> new_path;
    while (!path_buffer.empty())
    {
        new_path.push(path_buffer.top());
        path_buffer.pop();
    }
    predicted_path_ = new_path;
    return true;
}

bool MyRobot::isMovePossible(const Movement move, const Field from) const
{
    const int &x = from.x;
    const int &y = from.y;

    switch (move)
    {
    case Movement::Left:
        return predicted_sketch_.getVerticalWall(x - 1, y) != Border::Walled;
    case Movement::Right:
        return predicted_sketch_.getVerticalWall(x, y) != Border::Walled;
    case Movement::Up:
        return predicted_sketch_.getHorizontalWall(x, y - 1) != Border::Walled;
    case Movement::Down:
        return predicted_sketch_.getHorizontalWall(x, y) != Border::Walled;
    case Movement::None:
        return true;
    default:
        return false;
    }
}
