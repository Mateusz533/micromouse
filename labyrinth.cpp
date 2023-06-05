#include "labyrinth.h"
#include "modifiedwilsongenerator.h"
#include "myrobot.h"
#include <fstream>

Labyrinth::Labyrinth(const int size)
  : _sketch{ size }
  , p_generator{ new ModifiedWilsonGenerator() }
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
    p_generator->generateSketch(_sketch, _robot_position, _target_position);
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
