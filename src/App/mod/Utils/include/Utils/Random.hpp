#pragma once
#include <random>

namespace Lib::Utils {
class Random {
public:
    explicit Random();
    float range(float min, float max);
    int32_t range(int32_t min, int32_t max);

private:
    std::random_device m_device;
    std::mt19937 m_mt;
};
}