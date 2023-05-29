
#ifndef MAZESKETCH_H
#define MAZESKETCH_H

#include "field.h"
#include <vector>

template <typename T, T EXTERIOR_WALL>
class MazeSketch
{
  public:
    MazeSketch() = delete;
    MazeSketch(const int size = 16, const T fill = EXTERIOR_WALL);
    T getVerticalWall(const int index_x, const int index_y) const;
    T getHorizontalWall(const int index_x, const int index_y) const;
    T getWall(const Field &position, const Movement move) const;
    void setVerticalWall(const int index_x, const int index_y, const T value);
    void setHorizontalWall(const int index_x, const int index_y, const T value);
    void setWall(const Field &position, const Movement move, const T value);
    bool isInside(const Field &position) const;
    int size() const;

  private:
    std::vector<std::vector<T>> vertical_walls_;
    std::vector<std::vector<T>> horizontal_walls_;
};

template <typename T, T EXTERIOR_WALL>
MazeSketch<T, EXTERIOR_WALL>::MazeSketch(const int size, const T fill)
{
    const std::vector<T> column_v(size, fill);
    const std::vector<T> column_h(size - 1, fill);
    vertical_walls_ = std::vector<std::vector<T>>(size - 1, column_v);
    horizontal_walls_ = std::vector<std::vector<T>>(size, column_h);
}

template <typename T, T EXTERIOR_WALL>
T MazeSketch<T, EXTERIOR_WALL>::getVerticalWall(const int index_x,
                                                const int index_y) const
{
    const int size = horizontal_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size;
    return out_of_scope ? EXTERIOR_WALL : vertical_walls_[index_x][index_y];
}

template <typename T, T EXTERIOR_WALL>
T MazeSketch<T, EXTERIOR_WALL>::getHorizontalWall(const int index_x,
                                                  const int index_y) const
{
    const int size = horizontal_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1;
    return out_of_scope ? EXTERIOR_WALL : horizontal_walls_[index_x][index_y];
}

template <typename T, T EXTERIOR_WALL>
T MazeSketch<T, EXTERIOR_WALL>::getWall(const Field &position,
                                        const Movement move) const
{
    const int &x = position.x;
    const int &y = position.y;

    switch (move)
    {
    default:
        return EXTERIOR_WALL;
    case Movement::Left:
        return getVerticalWall(x - 1, y);
    case Movement::Right:
        return getVerticalWall(x, y);
    case Movement::Up:
        return getHorizontalWall(x, y - 1);
    case Movement::Down:
        return getHorizontalWall(x, y);
    }
}

template <typename T, T EXTERIOR_WALL>
void MazeSketch<T, EXTERIOR_WALL>::setVerticalWall(const int index_x,
                                                   const int index_y,
                                                   const T value)
{
    const int size = horizontal_walls_.size();
    if (index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size)
        return;
    vertical_walls_[index_x][index_y] = value;
}

template <typename T, T EXTERIOR_WALL>
void MazeSketch<T, EXTERIOR_WALL>::setHorizontalWall(const int index_x,
                                                     const int index_y,
                                                     const T value)
{
    const int size = horizontal_walls_.size();
    if (index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1)
        return;
    horizontal_walls_[index_x][index_y] = value;
}

template <typename T, T EXTERIOR_WALL>
void MazeSketch<T, EXTERIOR_WALL>::setWall(const Field &position,
                                           const Movement move, const T value)
{
    const int &x = position.x;
    const int &y = position.y;

    switch (move)
    {
    default:
        break;
    case Movement::Left:
        setVerticalWall(x - 1, y, value);
        break;
    case Movement::Right:
        setVerticalWall(x, y, value);
        break;
    case Movement::Up:
        setHorizontalWall(x, y - 1, value);
        break;
    case Movement::Down:
        setHorizontalWall(x, y, value);
        break;
    }
}

template <typename T, T EXTERIOR_WALL>
bool MazeSketch<T, EXTERIOR_WALL>::isInside(const Field &position) const
{
    const int &x = position.x;
    const int &y = position.y;
    const int size = this->size();
    return x >= 0 && x < size && y >= 0 && y < size;
}

template <typename T, T EXTERIOR_WALL>
int MazeSketch<T, EXTERIOR_WALL>::size() const
{
    const int h_size = horizontal_walls_.size();
    const int v_size = vertical_walls_.size();
    return v_size > h_size ? v_size : h_size;
}

#endif  // MAZESKETCH_H
