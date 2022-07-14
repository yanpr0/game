#include <cmath>

#include "utils.h"

namespace game
{

bool is_between(int value, int left, int right)
{
    return left <= value && value <= right;
}

float dist(Point a, Point b)
{
    auto x = a.x - b.x;
    auto y = a.y - b.y;
    return std::hypotf(x, y);
}

}

