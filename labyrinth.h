
#ifndef LABYRINTH_H
#define LABYRINTH_H

#include "robot.h"
#include <memory>
#include <string>
#include <vector>

class Labyrinth
{
  public:
    Labyrinth() = delete;
    Labyrinth(const int size = 16);
    void getFromFile(const std::string &path);
    void generateRandomSketch();
    void start();
    void pause();
    void restart();
    void setTargetPosition(const int x, const int y);
    void setRobotPosition(const int x, const int y);
    void step();

  private:
    void moveMouse(Movement movement);

    std::vector<std::vector<const bool>> vertical_walls_;
    std::vector<std::vector<const bool>> horizontal_walls_;
    std::unique_ptr<Robot> mouse_;
    int mouse_x_;
    int mouse_y_;
};

#endif  // LABYRINTH_H
