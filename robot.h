
#ifndef ROBOT_H
#define ROBOT_H

#include "movement.h"

/** \brief Robot 'micromouse'
 *
 *  Klasa interfejsowa, bazowa dla implementacji robotów 'micromouse'.
 */
class Robot
{
  public:
    ///  Wykonuje 1 automiczny ruch robota, po podaniu lokalnego położenia
    ///  ścian.
    virtual Movement run(const bool wall_left, const bool wall_right,
                         const bool wall_up, const bool wall_down) = 0;
};

#endif  // ROBOT_H
