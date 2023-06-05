#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "mazesketch.h"

/** \brief Generator labiryntu
 *
 *  Klasa interfejsowa, bazowa dla implementacji różnych generatorów.
 */
class MazeGenerator
{
  public:
    virtual void generateSketch(MazeSketch<bool, true> &sketch,
                                const Field &start_pose = Field(-1, -1),
                                const Field &target_pose = Field(-1,
                                                                 -1)) const = 0;
};

#endif  // MAZEGENERATOR_H
