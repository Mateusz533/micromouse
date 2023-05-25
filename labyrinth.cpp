
#include "labyrinth.h"
#include "myrobot.h"

Labyrinth::Labyrinth(const int size)
  : sketch_(size)
  , mouse_(new MyRobot(0, 0, size / 2, size / 2, size))
{
}

bool Labyrinth::getSketchFromFile(const std::string &path)
{
    // TODO: dodanie pobierania danych z pliku
    return false;
}

void Labyrinth::generateRandomSketch()
{
    // TODO: algorytm wyznaczania losowej ścieżki i wypełniania reszty
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
    // TODO: sprawdzanie, czy nie wpada na ściany
    switch (movement)
    {
    case Movement::Left:

        break;
    case Movement::Right:

        break;
    case Movement::Up:

        break;
    case Movement::Down:

        break;
    case Movement::None:

        break;
    }
}
