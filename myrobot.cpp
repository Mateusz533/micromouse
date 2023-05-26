
#include "myrobot.h"
#include <algorithm>
#include <set>
#include <stack>

MyRobot::MyRobot(const int start_x, const int start_y, const int target_x,
                 const int target_y, const int maze_size)
  : predicted_sketch_(maze_size, Border::Unknown)
  , position_x_(start_x)
  , position_y_(start_y)
  , target_x_(target_x)
  , target_y_(target_y)
{
}

Movement MyRobot::run(const bool wall_left, const bool wall_right,
                      const bool wall_up, const bool wall_down)
{
    updateBorderWalls(wall_left, wall_right, wall_up, wall_down);
    Movement move = getFloodMovement();
    updatePosition(move);
    return move;
}

void MyRobot::updateBorderWalls(const bool left, const bool right,
                                const bool up, const bool down)
{
    predicted_sketch_.setVerticalWall(position_x_, position_y_,
                                      right ? Border::Walled : Border::Empty);
    predicted_sketch_.setVerticalWall(position_x_ - 1, position_y_,
                                      left ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_x_, position_y_,
                                        down ? Border::Walled : Border::Empty);
    predicted_sketch_.setHorizontalWall(position_x_, position_y_ - 1,
                                        up ? Border::Walled : Border::Empty);
}

Movement MyRobot::getFloodMovement()
{
    if (target_x_ == position_x_ && target_y_ == position_y_)
        return Movement::None;

    if (!predicted_path_.empty())
    {
        const auto move = predicted_path_.front();
        if (isMovePossible(move))
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

std::vector<std::vector<Movement>> MyRobot::getDijkstraPaths()
{
    const int size = predicted_sketch_.size();
    std::vector visited(size, std::vector<bool>(size, false));
    std::vector distances(size, std::vector<unsigned int>(size, -1));
    std::vector pred(size, std::vector<Movement>(size, Movement::None));
    std::set<std::pair<int, int>> fields;
    std::pair<int, int> current_field(position_x_, position_y_);
    distances[position_x_][position_y_] = 0;
    fields.insert(std::pair(position_x_, position_y_));

    while (!fields.empty() && !(current_field.first == target_x_ &&
                                current_field.second == target_y_))
    {
        const auto itr = std::min_element(
          fields.begin(), fields.end(), [&distances](auto &left, auto &right) {
              return distances[left.first][left.second] <
                     distances[right.first][right.second];
          });
        current_field = *itr;
        fields.erase(current_field);
        const auto min_x = current_field.first;
        const auto min_y = current_field.second;

        std::vector<std::pair<int, int>> neighbors;

        if (isMovePossible(Movement::Left, min_x, min_y))
            neighbors.push_back(std::pair(min_x - 1, min_y));
        if (isMovePossible(Movement::Right, min_x, min_y))
            neighbors.push_back(std::pair(min_x + 1, min_y));
        if (isMovePossible(Movement::Up, min_x, min_y))
            neighbors.push_back(std::pair(min_x, min_y - 1));
        if (isMovePossible(Movement::Down, min_x, min_y))
            neighbors.push_back(std::pair(min_x, min_y + 1));

        for (const auto &n : neighbors)
        {
            const auto &x{ n.first };
            const auto &y{ n.second };
            if (!visited[x][y])
            {
                fields.insert(n);
                visited[x][y] = true;
            }
            if (distances[x][y] - 1 <= distances[min_x][min_y])
                continue;

            distances[x][y] = distances[min_x][min_y] + 1;
            pred[x][y] = x == min_x
                           ? (min_y > y ? Movement::Down : Movement::Up)
                           : (min_x > x ? Movement::Right : Movement::Left);
        }
    }

    return pred;
}

bool MyRobot::updatePath()
{
    const auto path_map = getDijkstraPaths();
    std::stack<Movement> path_buffer;
    int x = target_x_;
    int y = target_y_;
    while (x != position_x_ || y != position_y_)
    {
        switch (path_map[x][y])
        {
        case Movement::Left:
            path_buffer.push(Movement::Right);
            --x;
            break;
        case Movement::Right:
            path_buffer.push(Movement::Left);
            ++x;
            break;
        case Movement::Up:
            path_buffer.push(Movement::Down);
            --y;
            break;
        case Movement::Down:
            path_buffer.push(Movement::Up);
            ++y;
            break;
        default:
            return false;
        }
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

bool MyRobot::isMovePossible(Movement move, int x, int y)
{
    if (x == -1 || y == -1)
    {
        x = position_x_;
        y = position_y_;
    }

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

void MyRobot::updatePosition(Movement move)
{
    switch (move)
    {
    case Movement::Left:
        --position_x_;
        break;
    case Movement::Right:
        ++position_x_;
        break;
    case Movement::Up:
        --position_y_;
        break;
    case Movement::Down:
        ++position_y_;
        break;
    default:
        break;
    }
}
