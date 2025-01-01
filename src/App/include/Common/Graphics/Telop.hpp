#pragma once
#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
class Telop {
public:
    explicit Telop();

    void update();
    void draw();
    void destroy();
    bool isExpired() const;

    std::string text;
    Vector3 position;
    Vector3 color;
    float duration;

private:
    float m_elapsed;
};
}