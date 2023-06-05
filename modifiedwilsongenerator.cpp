#include "modifiedwilsongenerator.h"
#include <stack>

void ModifiedWilsonGenerator::generateSketch(MazeSketch<bool, true> &sketch,
                                             const Field &start_pose,
                                             const Field &target_pose)
{
    const int size{ sketch.size() };
    Sketch new_sketch(size, true);
    Matrix visited(size, std::vector<bool>(size, false));

    const Field start_field =
      sketch.isInside(start_pose) ? start_pose : Field(0, 0);
    const Field target_field =
      sketch.isInside(target_pose) ? target_pose : Field(size / 2, size / 2);

    generateMainPath(new_sketch, visited, start_field, target_field);
    generateMissingPaths(new_sketch, visited);
    sketch = std::move(new_sketch);
}

void ModifiedWilsonGenerator::generateMainPath(Sketch &sketch, Matrix &visited,
                                               const Field &start_field,
                                               const Field &target_field)
{
    generatePath(sketch, visited, start_field,
                 [&target_field](auto current_field) {
                     return current_field == target_field;
                 });
}

void ModifiedWilsonGenerator::generateMissingPaths(Sketch &sketch,
                                                   Matrix &visited)
{
    while (true)
    {
        const auto column =
          std::find_if(visited.begin(), visited.end(), [](auto &col) {
              return std::find(col.begin(), col.end(), false) != col.end();
          });

        if (column == visited.end())
            break;

        const auto row = std::find(column->begin(), column->end(), false);
        const int x = std::distance(visited.begin(), column);
        const int y = std::distance(column->begin(), row);

        generatePath(sketch, visited, Field(x, y),
                     [&visited](auto current_field) {
                         return visited[current_field.x][current_field.y];
                     });
    }
}

void ModifiedWilsonGenerator::generatePath(Sketch &sketch, Matrix &visited,
                                           const Field &start_field,
                                           std::function<bool(Field)> stop_cond)
{
    const int size{ sketch.size() };
    std::vector<Movement> possible_moves(
      { Movement::Left, Movement::Right, Movement::Up, Movement::Down });
    std::stack<Field> fields;
    Field current_field{ start_field };
    Matrix path(size, std::vector<bool>(size, false));
    fields.push(current_field);
    path[current_field.x][current_field.y] = true;

    while (!fields.empty() && !stop_cond(current_field))
    {
        std::random_shuffle(possible_moves.begin(), possible_moves.end());

        const auto &move_itr = std::find_if(
          possible_moves.begin(), possible_moves.end(),
          [&current_field, &path, &sketch](const auto &move) {
              const Field new_field{ current_field.positionAfterMove(move) };
              return sketch.isInside(new_field) &&
                     !path[new_field.x][new_field.y];
          });

        if (move_itr == possible_moves.end())
        {
            fields.pop();
            current_field = fields.top();
            continue;
        }

        sketch.setWall(current_field, *move_itr, false);
        current_field = current_field.positionAfterMove(*move_itr);
        path[current_field.x][current_field.y] = true;
        fields.push(current_field);
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            visited[i][j] = visited[i][j] || path[i][j];
        }
    }
}
