#ifndef MODIFIEDWILSONGENERATOR_H
#define MODIFIEDWILSONGENERATOR_H

#include "mazegenerator.h"
#include <functional>

/** \brief Przykład genetarora labiryntu
 *
 *  Autorska klasa implementująca generator labiryntu.
 */
class ModifiedWilsonGenerator : public MazeGenerator
{
  public:
    ModifiedWilsonGenerator() = default;
    void generateSketch(MazeSketch<bool, true> &sketch,
                        const Field &start_pose = Field(-1, -1),
                        const Field &target_pose = Field(-1,
                                                         -1)) const override;

  private:
    typedef MazeSketch<bool, true> Sketch;
    typedef std::vector<std::vector<bool>> Matrix;

    void generateMainPath(Sketch &sketch, Matrix &visited,
                          const Field &start_field,
                          const Field &target_field) const;
    void generateMissingPaths(Sketch &sketch, Matrix &visited) const;
    void generatePath(Sketch &sketch, Matrix &visited, const Field &start_field,
                      std::function<bool(Field)> stop_cond) const;
};

#endif  // MODIFIEDWILSONGENERATOR_H
