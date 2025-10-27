#pragma once
#include <App/library.hpp>
#include <cstdint>

namespace App::Common::Graphics {
/**
 * パーティクルを表すインターフェイスです。
 */
class IParticle {
public:
    enum class Type : int32_t {
        None = 0,
        Spherical,
        Plane,
        Count
    };

    explicit IParticle() = default;
    virtual ~IParticle() = default;

    /**
     * パーティクルを更新します。
     * 寿命の更新などが主に行われます。
     */
    virtual void update() = 0;

    /**
     * パーティクルを描画します。
     * 引数の頂点情報は常にボックスです。
     * @param vertexBuffer
     * @param indexBuffer
     * @param indexLength
     */
    virtual void draw(const std::shared_ptr<CpuBuffer>& vertexBuffer, const std::shared_ptr<CpuBuffer>& indexBuffer, int32_t indexLength) = 0;

    /**
     * パーティクルの保持していたリソースを破棄します。
     */
    virtual void destroy() = 0;

    /**
     * 寿命が切れたなら true を返します。
     * @return
     */
    virtual bool isExpired() const = 0;

private:
};
}