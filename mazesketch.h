
#ifndef MAZESKETCH_H
#define MAZESKETCH_H

#include <vector>

template <typename T, T DEFAULT_VALUE>
class MazeSketch
{
  public:
    MazeSketch() = delete;
    MazeSketch(const int size = 16, T fill = DEFAULT_VALUE);
    T getVerticalWall(const int index_x, const int index_y) const;
    T getHorizontalWall(const int index_x, const int index_y) const;
    void setVerticalWall(const int index_x, const int index_y, const T value);
    void setHorizontalWall(const int index_x, const int index_y, const T value);
    int size() const;

  private:
    std::vector<std::vector<T>> vertical_walls_;
    std::vector<std::vector<T>> horizontal_walls_;
};

template <typename T, T DEFAULT_VALUE>
MazeSketch<T, DEFAULT_VALUE>::MazeSketch(const int size, T fill)
{
    const std::vector<T> column_v(size, fill);
    const std::vector<T> column_h(size - 1, fill);
    vertical_walls_ = std::vector<std::vector<T>>(size - 1, column_v);
    horizontal_walls_ = std::vector<std::vector<T>>(size, column_h);
}

template <typename T, T DEFAULT_VALUE>
T MazeSketch<T, DEFAULT_VALUE>::getVerticalWall(const int index_x,
                                                const int index_y) const
{
    const int size = vertical_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size;
    return out_of_scope ? DEFAULT_VALUE : vertical_walls_[index_x][index_y];
}

template <typename T, T DEFAULT_VALUE>
T MazeSketch<T, DEFAULT_VALUE>::getHorizontalWall(const int index_x,
                                                  const int index_y) const
{
    const int size = vertical_walls_.size();
    const bool out_of_scope =
      index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1;
    return out_of_scope ? DEFAULT_VALUE : horizontal_walls_[index_x][index_y];
}

template <typename T, T DEFAULT_VALUE>
void MazeSketch<T, DEFAULT_VALUE>::setVerticalWall(const int index_x,
                                                   const int index_y,
                                                   const T value)
{
    const int size = vertical_walls_.size();
    if (index_x < 0 || index_x >= size - 1 || index_y < 0 || index_y >= size)
        return;
    vertical_walls_[index_x][index_y] = value;
}

template <typename T, T DEFAULT_VALUE>
void MazeSketch<T, DEFAULT_VALUE>::setHorizontalWall(const int index_x,
                                                     const int index_y,
                                                     const T value)
{
    const int size = vertical_walls_.size();
    if (index_x < 0 || index_x >= size || index_y < 0 || index_y >= size - 1)
        return;
    horizontal_walls_[index_x][index_y] = value;
}

template <typename T, T DEFAULT_VALUE>
int MazeSketch<T, DEFAULT_VALUE>::size() const
{
    return vertical_walls_.size();
}

#endif  // MAZESKETCH_H
