#pragma once
#include <cstdint>

namespace App::Common::Graphics {
class IParticle {
public:
    enum class Type : int32_t {
        None = 0,
        Spherical
    };

    explicit IParticle() = default;
    virtual ~IParticle() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void destroy() = 0;
    virtual bool isExpired() const = 0;
    virtual Type getType() const = 0;

private:
};
}