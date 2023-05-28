
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
    Sketch new_sketch(size, false);
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
    Sketch new_sketch(size, true);
    Matrix visited(size, std::vector<bool>(size, false));

    generateMainPath(new_sketch, visited);
    generateMissingPaths(new_sketch, visited);
    sketch_ = new_sketch;
}

void Labyrinth::generateMainPath(Sketch &sketch, Matrix &visited)
{
    generatePath(sketch, visited, mouse_position_, [this](auto current_field) {
        return current_field == this->target_position_;
    });
}

void Labyrinth::generateMissingPaths(Sketch &sketch, Matrix &visited)
{
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

        generatePath(sketch, visited, Field(x, y),
                     [&visited](auto current_field) {
                         return visited[current_field.x][current_field.y];
                     });
    }
}

void Labyrinth::generatePath(Sketch &sketch, Matrix &visited,
                             const Field start_field,
                             std::function<bool(Field)> stop_cond)
{
    const int size = sketch.size();
    const std::vector<std::pair<int, int>> possible_moves{
        { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 }
    };
    std::stack<Field> fields;
    Field current_field{ start_field };
    Matrix on_path(size, std::vector<bool>(size, false));
    fields.push(current_field);
    on_path[current_field.x][current_field.y] = true;

    while (!fields.empty() && !stop_cond(current_field))
    {
        const bool out_of_scope = std::none_of(
          possible_moves.begin(), possible_moves.end(),
          [&current_field, &size, &on_path](auto &value) {
              const int x = value.first + current_field.x;
              const int y = value.second + current_field.y;
              return x >= 0 && x < size && y >= 0 && y < size && !on_path[x][y];
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
            const int x = dx + current_field.x;
            const int y = dy + current_field.y;
            if (x < 0 || x >= size || y < 0 || y >= size || on_path[x][y])
                continue;

            if (dx != 0)
                sketch.setVerticalWall(std::min(x, x - dx), y, false);
            else
                sketch.setHorizontalWall(x, std::min(y, y - dy), false);

            current_field = Field(x, y);
            break;
        }

        on_path[current_field.x][current_field.y] = true;
        fields.push(current_field);
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            visited[i][j] = visited[i][j] || on_path[i][j];
        }
    }
}

void Labyrinth::restart()
{
    const int size = sketch_.size();
    mouse_.reset(new MyRobot(mouse_position_, target_position_, size));
}

bool Labyrinth::setTargetPosition(const int x, const int y)
{
    const int size = sketch_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    target_position_.x = x;
    target_position_.y = y;
    return true;
}
bool Labyrinth::setTargetPosition(const Field position)
{
    return setTargetPosition(position.x, position.y);
}

bool Labyrinth::setRobotPosition(const int x, const int y)
{
    const int size = sketch_.size();
    if (x < 0 || x > size || y < 0 || y > size)
        return false;

    mouse_position_.x = x;
    mouse_position_.y = y;
    return true;
}
bool Labyrinth::setRobotPosition(const Field position)
{
    return setRobotPosition(position.x, position.y);
}

Field Labyrinth::getTargetPosition() const
{
    return target_position_;
}

Field Labyrinth::getRobotPosition() const
{
    return mouse_position_;
}

MazeSketch<bool, true> Labyrinth::getSketch() const
{
    return sketch_;
}

void Labyrinth::step()
{
    const int &x = mouse_position_.x;
    const int &y = mouse_position_.y;

    const bool left = sketch_.getVerticalWall(x - 1, y);
    const bool right = sketch_.getVerticalWall(x, y);
    const bool up = sketch_.getHorizontalWall(x, y - 1);
    const bool down = sketch_.getHorizontalWall(x, y);
    const Movement movement = mouse_->run(left, right, up, down);
    moveMouse(movement);
}

void Labyrinth::moveMouse(const Movement movement)
{
    const int &x = mouse_position_.x;
    const int &y = mouse_position_.y;

    switch (movement)
    {
    case Movement::Left:
        if (!sketch_.getVerticalWall(x - 1, y))
            --mouse_position_.x;
        break;
    case Movement::Right:
        if (!sketch_.getVerticalWall(x, y))
            ++mouse_position_.x;
        break;
    case Movement::Up:
        if (!sketch_.getHorizontalWall(x, y - 1))
            --mouse_position_.y;
        break;
    case Movement::Down:
        if (!sketch_.getHorizontalWall(x, y))
            ++mouse_position_.y;
        break;
    case Movement::None:
        break;
    }
}
