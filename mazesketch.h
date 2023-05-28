
#ifndef MAZESKETCH_H
#define MAZESKETCH_H

#include <vector>

template <typename T, T EXTERIOR_VALUE>
class MazeSketch
{
  public:
    MazeSketch() = delete;
    MazeSketch(const int size = 16, const T fill = EXTERIOR_VALUE);
    T getVerticalWall(const int index_x, const int index_y) const;
    T getHorizontalWall(const int index_x, const int index_y) const;
    void setVerticalWall(const int index_x, const int index_y, const T value);
    void setHorizontalWall(const int index_x, const int index_y, const T value);
    int size() const;

  private:
    std::vector<std::vector<T>> vertical_walls_;
    std::vector<std::vector<T>> horizontal_walls_;
};

template <typename T, T EXTERIOR_VALUE>
MazeSketch<T, EXTERIOR_VALUE>::MazeSketch(const int size, const T fill)
{
    const std::vector<T> column_v(size, fill);
    const std::vector<T> column_h(size - 1, fill);
    vertical_walls_ = std::vector<std::vector<T>>(size - 1, column_v);
    horizontal_walls_ = std::vector<std::vector<T>>(size, column_h);
}

template <typename T, T EXTERIOR_VALUE>
T MazeSketch<T, EXTERIOR_VALUE>::getVerticalWall(const int index_x,
                                                 const int index_y) const
{
    const int size = horizontal_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size;
    return out_of_scope ? EXTERIOR_VALUE : vertical_walls_[index_x][index_y];
}

template <typename T, T EXTERIOR_VALUE>
T MazeSketch<T, EXTERIOR_VALUE>::getHorizontalWall(const int index_x,
                                                   const int index_y) const
{
    const int size = horizontal_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1;
    return out_of_scope ? EXTERIOR_VALUE : horizontal_walls_[index_x][index_y];
}

template <typename T, T EXTERIOR_VALUE>
void MazeSketch<T, EXTERIOR_VALUE>::setVerticalWall(const int index_x,
                                                    const int index_y,
                                                    const T value)
{
    const int size = horizontal_walls_.size();
    if (index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size)
        return;
    vertical_walls_[index_x][index_y] = value;
}

template <typename T, T EXTERIOR_VALUE>
void MazeSketch<T, EXTERIOR_VALUE>::setHorizontalWall(const int index_x,
                                                      const int index_y,
                                                      const T value)
{
    const int size = horizontal_walls_.size();
    if (index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1)
        return;
    horizontal_walls_[index_x][index_y] = value;
}

template <typename T, T EXTERIOR_VALUE>
int MazeSketch<T, EXTERIOR_VALUE>::size() const
{
    const int h = horizontal_walls_.size();
    const int v = vertical_walls_.size();
    return v > h ? v : h;
}

#endif  // MAZESKETCH_H
