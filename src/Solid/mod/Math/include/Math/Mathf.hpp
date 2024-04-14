#pragma once

namespace Lib::Math {
class Mathf {
public:
    static float min(float a, float b);
    static float max(float a, float b);
    static float map(float min, float max, float t);
    static float clamp(float min, float max, float current);
    static float sin(float v);
    static float cos(float v);
    static float tan(float v);
    static float equals(float a, float b);

private:
    Mathf() = delete;
    ~Mathf() = delete;
};
}