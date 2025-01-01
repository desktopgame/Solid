#pragma once

namespace App::Common::Graphics {
class IParticle {
public:
    explicit IParticle() = default;
    virtual ~IParticle() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void destroy() = 0;
    virtual bool isExpired() const = 0;

private:
};
}