#pragma once

namespace Lib::Math {
class Mathf {
public:
    inline static constexpr float Deg2Rad = 3.14f / 180.0f;
    inline static constexpr float Rad2Deg = 180.0f / 3.14f;

    static float min(float a, float b);
    static float max(float a, float b);
    static float map(float min, float max, float t);
    static float clamp(float min, float max, float current);
    static float sin(float v);
    static float cos(float v);
    static float tan(float v);
    static float sqrt(float v);
    static float equals(float a, float b);

private:
    Mathf() = delete;
    ~Mathf() = delete;
};
}