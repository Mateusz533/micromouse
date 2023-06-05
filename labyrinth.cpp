
#include "labyrinth.h"
#include "myrobot.h"
#include <algorithm>
#include <fstream>
#include <stack>

Labyrinth::Labyrinth(const int size)
  : _sketch{ size }
  , p_robot{ new MyRobot(0, 0, size / 2, size / 2, size) }
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

    const int size{ _sketch.size() };
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
                new_sketch.setWall(Field(j, i), Movement::Down, true);
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
                new_sketch.setWall(Field(j, i), Movement::Right, true);
                break;
            default:
                stream.close();
                return false;
            }
        }
    }

    stream.close();
    _sketch = new_sketch;
    return true;
}

void Labyrinth::generateRandomSketch()
{
    const int size{ _sketch.size() };
    Sketch new_sketch(size, true);
    Matrix visited(size, std::vector<bool>(size, false));

    generateMainPath(new_sketch, visited);
    generateMissingPaths(new_sketch, visited);
    _sketch = new_sketch;
}

void Labyrinth::generateMainPath(Sketch &sketch, Matrix &visited)
{
    generatePath(sketch, visited, _robot_position, [this](auto current_field) {
        return current_field == this->_target_position;
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
                             const Field &start_field,
                             std::function<bool(Field)> stop_cond)
{
    const int size{ sketch.size() };
    std::vector<Movement> possible_moves(
      { Movement::Left, Movement::Right, Movement::Up, Movement::Down });
    std::stack<Field> fields;
    Field current_field{ start_field };
    Matrix path(size, std::vector<bool>(size, false));
    fields.push(current_field);
    path[current_field.x][current_field.y] = true;

    while (!fields.empty() && !stop_cond(current_field))
    {
        std::random_shuffle(possible_moves.begin(), possible_moves.end());

        const auto &move_itr = std::find_if(
          possible_moves.begin(), possible_moves.end(),
          [&current_field, &path, &sketch](const auto &move) {
              const Field new_field{ current_field.positionAfterMove(move) };
              return sketch.isInside(new_field) &&
                     !path[new_field.x][new_field.y];
          });

        if (move_itr == possible_moves.end())
        {
            fields.pop();
            current_field = fields.top();
            continue;
        }

        sketch.setWall(current_field, *move_itr, false);
        current_field = current_field.positionAfterMove(*move_itr);
        path[current_field.x][current_field.y] = true;
        fields.push(current_field);
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            visited[i][j] = visited[i][j] || path[i][j];
        }
    }
}

void Labyrinth::restart()
{
    const int size{ _sketch.size() };
    p_robot.reset(new MyRobot(_robot_position, _target_position, size));
}

bool Labyrinth::setTargetPosition(const int x, const int y)
{
    return setTargetPosition(Field(x, y));
}
bool Labyrinth::setTargetPosition(const Field &position)
{
    if (!_sketch.isInside(position))
        return false;

    _target_position = position;
    return true;
}

bool Labyrinth::setRobotPosition(const int x, const int y)
{
    return setRobotPosition(Field(x, y));
}
bool Labyrinth::setRobotPosition(const Field &position)
{
    if (!_sketch.isInside(position))
        return false;

    _robot_position = position;
    return true;
}

Field Labyrinth::getTargetPosition() const
{
    return _target_position;
}

Field Labyrinth::getRobotPosition() const
{
    return _robot_position;
}

MazeSketch<bool, true> Labyrinth::getSketch() const
{
    return _sketch;
}

void Labyrinth::step()
{
    const bool left{ _sketch.getWall(_robot_position, Movement::Left) };
    const bool right{ _sketch.getWall(_robot_position, Movement::Right) };
    const bool up{ _sketch.getWall(_robot_position, Movement::Up) };
    const bool down{ _sketch.getWall(_robot_position, Movement::Down) };
    const Movement movement = p_robot->run(left, right, up, down);

    if (!_sketch.getWall(_robot_position, movement))
        _robot_position = _robot_position.positionAfterMove(movement);
}
