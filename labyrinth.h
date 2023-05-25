
#ifndef LABYRINTH_H
#define LABYRINTH_H

#include "mazesketch.h"
#include "robot.h"
#include <memory>
#include <string>
#include <vector>

class Labyrinth
{
  public:
    Labyrinth() = delete;
    Labyrinth(const int size = 16);
    bool getSketchFromFile(const std::string &path);
    void generateRandomSketch();
    void restart();
    bool setTargetPosition(const int x, const int y);
    bool setRobotPosition(const int x, const int y);
    std::pair<int, int> getTargetPosition() const;
    std::pair<int, int> getRobotPosition() const;
    MazeSketch<bool, true> getSketch() const;
    void step();

  private:
    void moveMouse(Movement movement);

    MazeSketch<bool, true> sketch_;
    std::unique_ptr<Robot> mouse_;
    int mouse_x_{ 0 };
    int mouse_y_{ 0 };
    int target_x_{ 8 };
    int target_y_{ 8 };
};

#endif  // LABYRINTH_H
