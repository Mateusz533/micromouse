#ifndef FLOODMOVEMENTGENERATOR_H
#define FLOODMOVEMENTGENERATOR_H

#include "movementgenerator.h"
#include <functional>
#include <queue>

/** \brief Powodziowy generator ruchu
 *
 *  Implementacja generatora ruchu wykorzystującego algorytm powodziowy.
 */
class FloodMovementGenerator : public MovementGenerator
{
  public:
    FloodMovementGenerator() = delete;
    FloodMovementGenerator(
      const int size, const Field &start, const Field &target,
      std::function<bool(const Movement &, const Field &)> isMovePossible);

    Movement nextMove() override;
    Field getPosition() const override;

  private:
    bool updatePath();
    std::vector<std::vector<Movement>> getDijkstraPaths() const;
    std::function<bool(const Movement &, const Field &)> isMovePossible;

    const int _size;
    const Field _target;
    Field _position;
    std::queue<Movement> _predicted_path;
};

#endif  // FLOODMOVEMENTGENERATOR_H
