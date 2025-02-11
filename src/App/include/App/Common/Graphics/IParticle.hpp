#pragma once
#include <App/library.hpp>
#include <cstdint>


namespace App::Common::Graphics {
class IParticle {
public:
    enum class Type : int32_t {
        None = 0,
        Spherical,
        Count
    };

    explicit IParticle() = default;
    virtual ~IParticle() = default;

    virtual void update() = 0;
    virtual void draw(const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength) = 0;
    virtual void destroy() = 0;
    virtual bool isExpired() const = 0;

private:
};
}