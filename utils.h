#ifndef UTILS_YANPRO
#define UTILS_YANPRO

#include <type_traits>

namespace game
{

struct Point
{
    int x;
    int y;

    bool operator==(const Point&) const = default;
};

template<typename T>
requires std::is_arithmetic_v<T>
T sqr(T x)
{
    return x * x;
}

bool is_between(int value, int left, int right);

float dist(Point a, Point b);

}

#endif

