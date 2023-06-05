#ifndef LABYRINTH_H
#define LABYRINTH_H

#include "field.h"
#include "mazegenerator.h"
#include "mazesketch.h"
#include "robot.h"
#include <memory>
#include <string>
#include <vector>

/** \brief Labirynt
 *
 *  Klasa obsługująca logikę symulacji zawodów 'micromouse'.
 */
class Labyrinth
{
  public:
    Labyrinth() = delete;
    Labyrinth(const int size = 16);
    /// \return poprawność wyczytania szkicu
    bool getSketchFromFile(const std::string &path);
    void generateRandomSketch();
    void restart();
    /**
     *  Ignoruje nieprawidłowe dane.
     *  \return poprawność ustawienia pozycji
     */
    bool setTargetPosition(const int x, const int y);
    /**
     *  Ignoruje nieprawidłowe dane.
     *  \return poprawność ustawienia pozycji
     */
    bool setTargetPosition(const Field &position);
    /**
     *  Ignoruje nieprawidłowe dane.
     *  \return poprawność ustawienia pozycji
     */
    bool setRobotPosition(const int x, const int y);
    /**
     *  Ignoruje nieprawidłowe dane.
     *  \return poprawność ustawienia pozycji
     */
    bool setRobotPosition(const Field &position);
    Field getTargetPosition() const;
    Field getRobotPosition() const;
    MazeSketch<bool, true> getSketch() const;
    /// Wykonuje ruch robota o 1 pole, jeśli taki nastąpi.
    void step();

  private:
    MazeSketch<bool, true> _sketch;
    std::unique_ptr<MazeGenerator> p_generator;
    std::unique_ptr<Robot> p_robot;
    Field _robot_position{ 0, 0 };
    Field _target_position{ 7, 7 };
};

#endif  // LABYRINTH_H
