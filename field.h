
#ifndef FIELD_H
#define FIELD_H
#include "movement.h"

struct Field
{
    Field() = delete;
    Field(int x, int y)
      : x(x)
      , y(y){};
    bool operator==(const Field &right) const
    {
        return x == right.x && y == right.y;
    };
    bool operator!=(const Field &right) const
    {
        return x != right.x || y != right.y;
    };
    bool operator<(const Field &right) const
    {
        return x != right.x ? x < right.x : y < right.y;
    };

    Field positionAfterMove(const Movement move)
    {
        switch (move)
        {
        case Movement::Left:
            return Field{ x - 1, y };
        case Movement::Right:
            return Field{ x + 1, y };
        case Movement::Up:
            return Field{ x, y - 1 };
        case Movement::Down:
            return Field{ x, y + 1 };
        default:
            return Field{ *this };
        }
    }
    Field positionBeforeMove(const Movement move)
    {
        switch (move)
        {
        case Movement::Left:
            return Field{ x + 1, y };
        case Movement::Right:
            return Field{ x - 1, y };
        case Movement::Up:
            return Field{ x, y + 1 };
        case Movement::Down:
            return Field{ x, y - 1 };
        default:
            return Field{ *this };
        }
    }

    int x{ 0 };
    int y{ 0 };
};

#endif  // FIELD_H
