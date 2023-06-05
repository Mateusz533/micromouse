#ifndef MOVEMENTGENERATOR_H
#define MOVEMENTGENERATOR_H

#include "field.h"
#include "movement.h"

/** \brief Generator ruchu w labiryncie
 *
 *  Klasa interfejsowa, bazowa dla implementacji różnych algorytmów ruchu.
 */
class MovementGenerator
{
  public:
    virtual Movement nextMove() = 0;
    virtual Field getPosition() const = 0;
};

#endif  // MOVEMENTGENERATOR_H
