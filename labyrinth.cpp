
#include "labyrinth.h"
#include "myrobot.h"
#include <algorithm>
#include <fstream>
#include <stack>

Labyrinth::Labyrinth(const int size)
  : sketch_(size)
  , mouse_(new MyRobot(0, 0, size / 2, size / 2, size))
{
    std::srand(time(0));
}

bool Labyrinth::getSketchFromFile(const std::string &path)
{
    std::ifstream stream;
    stream.open(path, std::ios::binary);
    stream.unsetf(std::ios_base::skipws);
    if (!stream.is_open())
        return false;

    const int size = sketch_.size();
    MazeSketch<bool, true> new_sketch(size, false);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            char sign;
            stream >> sign;
            if (sign == '\r')
                stream >> sign;
            if (sign == '\n' || sign == std::char_traits<char>::eof())
                break;
            switch (sign)
            {
            case ' ':
                break;
            case '_':
                new_sketch.setHorizontalWall(j, i, true);
                break;
            default:
                stream.close();
                return false;
            }

            stream >> sign;
            if (sign == '\r')
                stream >> sign;
            if (sign == '\n' || sign == std::char_traits<char>::eof())
                break;
            switch (sign)
            {
            case ' ':
                break;
            case '|':
                new_sketch.setVerticalWall(j, i, true);
                break;
            default:
                stream.close();
                return false;
            }
        }
    }

    stream.close();
    sketch_ = new_sketch;
    return true;
}

void Labyrinth::generateRandomSketch()
{
    const int size = sketch_.size();
    MazeSketch<bool, true> new_sketch(size, true);
    std::vector visited(size, std::vector<bool>(size, false));

    generateMainPath(new_sketch, visited);
    generateMissingPaths(new_sketch, visited);
    sketch_ = new_sketch;
}

void Labyrinth::generateMainPath(MazeSketch<bool, true> &sketch,
                                 std::vector<std::vector<bool>> &visited)
{
    const int size = sketch.size();
    const std::vector<std::pair<int, int>> possible_moves{
        { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }
    };

    std::stack<std::pair<int, int>> fields;
    std::pair<int, int> current_field(mouse_x_, mouse_y_);
    fields.push(current_field);
    visited[mouse_x_][mouse_y_] = true;

    while (!fields.empty() && (current_field.first != target_x_ ||
                               current_field.second != target_y_))
    {
        const bool out_of_scope = std::none_of(
          possible_moves.begin(), possible_moves.end(),
          [&current_field, &size, &visited](auto &value) {
              const int x = value.first + current_field.first;
              const int y = value.second + current_field.second;
              return x >= 0 && x < size && y >= 0 && y < size && !visited[x][y];
          });

        if (out_of_scope)
        {
            fields.pop();
            current_field = fields.top();
            continue;
        }

        do
        {
            const int rand = std::rand() % 4 - 1;
            const int dx = rand % 2;
            const int dy = (rand - 1) % 2;
            const int x = dx + current_field.first;
            const int y = dy + current_field.second;
            if (x < 0 || x >= size || y < 0 || y >= size || visited[x][y])
                continue;

            if (dx != 0)
                sketch.setVerticalWall(std::min(x, x - dx), y, false);
            else
                sketch.setHorizontalWall(x, std::min(y, y - dy), false);

            current_field = std::pair<int, int>(x, y);
            visited[x][y] = true;
            fields.push(current_field);
        } while (false);
    }
}

void Labyrinth::generateMissingPaths(MazeSketch<bool, true> &sketch,
                                     std::vector<std::vector<bool>> &visited)
{
    const int size = sketch.size();
    const std::vector<std::pair<int, int>> possible_moves{
        { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }
    };

    while (true)
    {
        const auto column =
          std::find_if(visited.begin(), visited.end(), [](auto &col) {
              return std::find(col.begin(), col.end(), false) != col.end();
          });

        if (column == visited.end())
            break;

        const auto row = std::find(column->begin(), column->end(), false);
        const int x = std::distance(visited.begin(), column);
        const int y = std::distance(column->begin(), row);
        std::stack<std::pair<int, int>> fields;
        std::pair<int, int> current_field(x, y);
        std::vector on_path(size, std::vector<bool>(size, false));
        fields.push(current_field);
        visited[x][y] = true;
        on_path[x][y] = true;

        while (!fields.empty())
        {
            const bool out_of_scope =
              std::none_of(possible_moves.begin(), possible_moves.end(),
                           [&current_field, &size, &on_path](auto &value) {
                               const int x = value.first + current_field.first;
                               const int y =
                                 value.second + current_field.second;
                               return x >= 0 && x < size && y >= 0 &&
                                      y < size && !on_path[x][y];
                           });

            if (out_of_scope)
            {
                fields.pop();
                current_field = fields.top();
                continue;
            }

            const long int rand = std::rand();

            for (int i = 0; i < 4; ++i)
            {
                const int r = (rand + i) % 4 - 1;
                const int dx = r % 2;
                const int dy = (r - 1) % 2;
                const int x = dx + current_field.first;
                const int y = dy + current_field.second;
                if (x < 0 || x >= size || y < 0 || y >= size || on_path[x][y])
                    continue;

                if (dx != 0)
                    sketch.setVerticalWall(std::min(x, x - dx), y, false);
                else
                    sketch.setHorizontalWall(x, std::min(y, y - dy), false);

                current_field = std::pair<int, int>(x, y);
                break;
            }

            if (visited[current_field.first][current_field.second])
                break;

            visited[current_field.first][current_field.second] = true;
            on_path[current_field.first][current_field.second] = true;
            fields.push(current_field);
        }
    }
}

void Labyrinth::restart()
{
    const int size = sketch_.size();
    mouse_.reset(new MyRobot(mouse_x_, mouse_y_, target_x_, target_y_, size));
}

bool Labyrinth::setTargetPosition(const int x, const int y)
{
    const int size = sketch_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    target_x_ = x;
    target_y_ = y;
    return true;
}

bool Labyrinth::setRobotPosition(const int x, const int y)
{
    const int size = sketch_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    mouse_x_ = x;
    mouse_y_ = y;
    return true;
}

std::pair<int, int> Labyrinth::getTargetPosition() const
{
    return std::pair<int, int>(target_x_, target_y_);
}

std::pair<int, int> Labyrinth::getRobotPosition() const
{
    return std::pair<int, int>(mouse_x_, mouse_y_);
}

MazeSketch<bool, true> Labyrinth::getSketch() const
{
    return sketch_;
}

void Labyrinth::step()
{
    const bool left = sketch_.getVerticalWall(mouse_x_ - 1, mouse_y_);
    const bool right = sketch_.getVerticalWall(mouse_x_, mouse_y_);
    const bool up = sketch_.getHorizontalWall(mouse_x_, mouse_y_ - 1);
    const bool down = sketch_.getHorizontalWall(mouse_x_, mouse_y_);
    const Movement movement = mouse_->run(left, right, up, down);
    moveMouse(movement);
}

void Labyrinth::moveMouse(Movement movement)
{
    switch (movement)
    {
    case Movement::Left:
        if (!sketch_.getVerticalWall(mouse_x_ - 1, mouse_y_))
            --mouse_x_;
        break;
    case Movement::Right:
        if (!sketch_.getVerticalWall(mouse_x_, mouse_y_))
            ++mouse_x_;
        break;
    case Movement::Up:
        if (!sketch_.getHorizontalWall(mouse_x_, mouse_y_ - 1))
            --mouse_y_;
        break;
    case Movement::Down:
        if (!sketch_.getHorizontalWall(mouse_x_, mouse_y_))
            ++mouse_y_;
        break;
    case Movement::None:
        break;
    }
}
