#include <Math/Mathf.hpp>
#include <cmath>
#include <limits>

namespace Lib::Math {

float Mathf::min(float a, float b)
{
    return a < b ? a : b;
}
float Mathf::max(float a, float b)
{
    return a > b ? a : b;
}
float Mathf::map(float min, float max, float t)
{
    return min + ((max - min) * t);
}
float Mathf::clamp(float min, float max, float current)
{
    if (current < min) {
        current = min;
    }
    if (current > max) {
        current = max;
    }
    return current;
}
float Mathf::sin(float v)
{
    return ::sinf(v);
}
float Mathf::cos(float v)
{
    return ::cosf(v);
}
float Mathf::tan(float v)
{
    return ::tanf(v);
}
float Mathf::equals(float a, float b)
{
    float diff = std::abs(a - b);
    return diff <= std::numeric_limits<float>::epsilon();
}
}