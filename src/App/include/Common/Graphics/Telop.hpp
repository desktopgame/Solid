#pragma once
#include <library.hpp>
#include <memory>

namespace App::Common::Graphics {
class Telop {
public:
    static inline constexpr int32_t k_fontSize = 32;

    explicit Telop();

    void initialize();
    void update();
    void draw(const std::shared_ptr<FontMap>& fontMap, const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength);
    void destroy();
    bool isExpired() const;

    std::u16string text;
    Vector3 position;
    Vector3 color;
    float duration;

private:
    float m_elapsed;
};
}