#include <Utils/Random.hpp>

namespace Lib::Utils {
Random::Random()
    : m_device()
    , m_mt(m_device())
{
}
float Random::range(float min, float max)
{
    std::uniform_real_distribution<float> D(min, max);
    return D(m_mt);
}
int32_t Random::range(int32_t min, int32_t max)
{
    std::uniform_int_distribution<> D(min, max);
    return D(m_mt);
}
}