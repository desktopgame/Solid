#pragma once
#include <Graphics/TileBuffer.hpp>
#include <Math/Matrix.hpp>
#include <Math/Vector.hpp>
#include <array>
#include <memory>

#if SOLID_ENABLE_INTERNAL
#include <d3d12.h>
#include <wrl/client.h>
#endif

namespace Lib::Graphics {
class Shader;
class Buffer;
class Constant;
class TileBatch {
public:
    static std::shared_ptr<TileBatch> create(const std::shared_ptr<ITileBuffer> tileBuffer);
    ~TileBatch();

    int32_t rent();
    void release(int32_t index);

    void setTiles(int32_t index, const Math::Vector4* tiles, int32_t tileCount);
    const Math::Vector4* getTiles(int32_t index) const;
    int32_t getTileSize() const;

    void setMatrix(int32_t index, const Math::Matrix& matrix);
    Math::Matrix getMatrix(int32_t index) const;

    void setGlobalViewMatrix(const Math::Matrix& matrix);
    Math::Matrix getGlobalViewMatrix() const;

    void setGlobalProjectionMatrix(const Math::Matrix& matrix);
    Math::Matrix getGlobalProjectionMatrix() const;

#if SOLID_ENABLE_INTERNAL
    void render(
        const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
#endif

    inline static const std::array<Math::Matrix, 6> s_translateMatrixTable = {
        // posY
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.5f, 0.0f })),
        // negY
        Math::Matrix::translate(Math::Vector3({ 0.0f, -0.5f, 0.0f })),
        // posX
        Math::Matrix::translate(Math::Vector3({ 0.5f, 0.0f, 0.0f })),
        // negX
        Math::Matrix::translate(Math::Vector3({ -0.5f, 0.0f, 0.0f })),
        // posZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, 0.5f })),
        // negZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, -0.5f })),
    };

    inline static const std::array<Math::Matrix, 6> s_rotationMatrixTable = {
        // posY
        Math::Matrix::rotateX(-90.0f),
        // negY
        Math::Matrix::rotateX(90.0f),
        // posX
        Math::Matrix::rotateY(90.0f),
        // negX
        Math::Matrix::rotateY(-90.0f),
        // posZ
        Math::Matrix::rotateY(180.0f),
        // negZ
        Math::Matrix(),
    };

    inline static const std::array<Math::Vector3, 6> s_normalVectorTable = {
        // posY
        Math::Vector3({ 0, 1, 0 }),
        // negY
        Math::Vector3({ 0, -1, 0 }),
        // posX
        Math::Vector3({ 1, 0, 0 }),
        // negX
        Math::Vector3({ -1, 0, 0 }),
        // posZ
        Math::Vector3({ 0, 0, 1 }),
        // negZ
        Math::Vector3({ 0, 0, -1 }),
    };

    inline static const std::array<int32_t, 6> s_tileReverseTable = {
        // posY
        1,
        // negY
        0,
        // posX
        3,
        // negX
        2,
        // posZ
        5,
        // negZ
        4,
    };

    inline static const std::array<Math::Vector3, 64> s_colorTable = {
        Math::Vector3({ 1.0f, 1.0f / 16.0f, 1.0f / 16.0f }),
        Math::Vector3({ 1.0f, 2.0f / 16.0f, 2.0f / 16.0f }),
        Math::Vector3({ 1.0f, 3.0f / 16.0f, 3.0f / 16.0f }),
        Math::Vector3({ 1.0f, 4.0f / 16.0f, 4.0f / 16.0f }),
        Math::Vector3({ 1.0f, 5.0f / 16.0f, 5.0f / 16.0f }),
        Math::Vector3({ 1.0f, 6.0f / 16.0f, 6.0f / 16.0f }),
        Math::Vector3({ 1.0f, 7.0f / 16.0f, 7.0f / 16.0f }),
        Math::Vector3({ 1.0f, 8.0f / 16.0f, 8.0f / 16.0f }),
        Math::Vector3({ 1.0f, 9.0f / 16.0f, 9.0f / 16.0f }),
        Math::Vector3({ 1.0f, 10.0f / 16.0f, 10.0f / 16.0f }),
        Math::Vector3({ 1.0f, 11.0f / 16.0f, 11.0f / 16.0f }),
        Math::Vector3({ 1.0f, 12.0f / 16.0f, 12.0f / 16.0f }),
        Math::Vector3({ 1.0f, 13.0f / 16.0f, 13.0f / 16.0f }),
        Math::Vector3({ 1.0f, 14.0f / 16.0f, 14.0f / 16.0f }),
        Math::Vector3({ 1.0f, 15.0f / 16.0f, 15.0f / 16.0f }),
        Math::Vector3({ 1.0f, 16.0f / 16.0f, 16.0f / 16.0f }),

        Math::Vector3({ 1.0f / 16.0f, 1.0f, 1.0f / 16.0f }),
        Math::Vector3({ 2.0f / 16.0f, 1.0f, 2.0f / 16.0f }),
        Math::Vector3({ 3.0f / 16.0f, 1.0f, 3.0f / 16.0f }),
        Math::Vector3({ 4.0f / 16.0f, 1.0f, 4.0f / 16.0f }),
        Math::Vector3({ 5.0f / 16.0f, 1.0f, 5.0f / 16.0f }),
        Math::Vector3({ 6.0f / 16.0f, 1.0f, 6.0f / 16.0f }),
        Math::Vector3({ 7.0f / 16.0f, 1.0f, 7.0f / 16.0f }),
        Math::Vector3({ 8.0f / 16.0f, 1.0f, 8.0f / 16.0f }),
        Math::Vector3({ 9.0f / 16.0f, 1.0f, 9.0f / 16.0f }),
        Math::Vector3({ 10.0f / 16.0f, 1.0f, 10.0f / 16.0f }),
        Math::Vector3({ 11.0f / 16.0f, 1.0f, 11.0f / 16.0f }),
        Math::Vector3({ 12.0f / 16.0f, 1.0f, 12.0f / 16.0f }),
        Math::Vector3({ 13.0f / 16.0f, 1.0f, 13.0f / 16.0f }),
        Math::Vector3({ 14.0f / 16.0f, 1.0f, 14.0f / 16.0f }),
        Math::Vector3({ 15.0f / 16.0f, 1.0f, 15.0f / 16.0f }),
        Math::Vector3({ 16.0f / 16.0f, 1.0f, 16.0f / 16.0f }),

        Math::Vector3({ 1.0f / 16.0f, 1.0f / 16.0f, 1.0f }),
        Math::Vector3({ 2.0f / 16.0f, 2.0f / 16.0f, 1.0f }),
        Math::Vector3({ 3.0f / 16.0f, 3.0f / 16.0f, 1.0f }),
        Math::Vector3({ 4.0f / 16.0f, 4.0f / 16.0f, 1.0f }),
        Math::Vector3({ 5.0f / 16.0f, 5.0f / 16.0f, 1.0f }),
        Math::Vector3({ 6.0f / 16.0f, 6.0f / 16.0f, 1.0f }),
        Math::Vector3({ 7.0f / 16.0f, 7.0f / 16.0f, 1.0f }),
        Math::Vector3({ 8.0f / 16.0f, 8.0f / 16.0f, 1.0f }),
        Math::Vector3({ 9.0f / 16.0f, 9.0f / 16.0f, 1.0f }),
        Math::Vector3({ 10.0f / 16.0f, 10.0f / 16.0f, 1.0f }),
        Math::Vector3({ 11.0f / 16.0f, 11.0f / 16.0f, 1.0f }),
        Math::Vector3({ 12.0f / 16.0f, 12.0f / 16.0f, 1.0f }),
        Math::Vector3({ 13.0f / 16.0f, 13.0f / 16.0f, 1.0f }),
        Math::Vector3({ 14.0f / 16.0f, 14.0f / 16.0f, 1.0f }),
        Math::Vector3({ 15.0f / 16.0f, 15.0f / 16.0f, 1.0f }),
        Math::Vector3({ 16.0f / 16.0f, 16.0f / 16.0f, 1.0f }),

        Math::Vector3({ 1.0f / 16.0f, 1.0f / 16.0f, 1.0f / 16.0f }),
        Math::Vector3({ 2.0f / 16.0f, 2.0f / 16.0f, 2.0f / 16.0f }),
        Math::Vector3({ 3.0f / 16.0f, 3.0f / 16.0f, 3.0f / 16.0f }),
        Math::Vector3({ 4.0f / 16.0f, 4.0f / 16.0f, 4.0f / 16.0f }),
        Math::Vector3({ 5.0f / 16.0f, 5.0f / 16.0f, 5.0f / 16.0f }),
        Math::Vector3({ 6.0f / 16.0f, 6.0f / 16.0f, 6.0f / 16.0f }),
        Math::Vector3({ 7.0f / 16.0f, 7.0f / 16.0f, 7.0f / 16.0f }),
        Math::Vector3({ 8.0f / 16.0f, 8.0f / 16.0f, 8.0f / 16.0f }),
        Math::Vector3({ 9.0f / 16.0f, 9.0f / 16.0f, 9.0f / 16.0f }),
        Math::Vector3({ 10.0f / 16.0f, 10.0f / 16.0f, 10.0f / 16.0f }),
        Math::Vector3({ 11.0f / 16.0f, 11.0f / 16.0f, 11.0f / 16.0f }),
        Math::Vector3({ 12.0f / 16.0f, 12.0f / 16.0f, 12.0f / 16.0f }),
        Math::Vector3({ 13.0f / 16.0f, 13.0f / 16.0f, 13.0f / 16.0f }),
        Math::Vector3({ 14.0f / 16.0f, 14.0f / 16.0f, 14.0f / 16.0f }),
        Math::Vector3({ 15.0f / 16.0f, 15.0f / 16.0f, 15.0f / 16.0f }),
        Math::Vector3({ 16.0f / 16.0f, 16.0f / 16.0f, 16.0f / 16.0f }),
    };

private:
    TileBatch();

    struct CameraData {
    public:
        explicit CameraData() = default;
        Math::Matrix viewMatrix;
        Math::Matrix projectionMatrix;
        std::array<float, 32> padding;
    };
    static_assert(sizeof(CameraData) % 256 == 0);

    struct TransformData {
    public:
        explicit TransformData() = default;
        std::array<Math::Matrix, 6> translateMatrixTable;
        std::array<Math::Matrix, 6> rotationMatrixTable;
    };
    static_assert(sizeof(TransformData) % 256 == 0);

    struct ColorData {
    public:
        explicit ColorData() = default;
        std::array<Math::Vector3, 64> colorTable;
    };
    static_assert(sizeof(ColorData) % 256 == 0);

    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
    std::shared_ptr<Buffer> m_constantBuffer;
    std::shared_ptr<Buffer> m_commandBuffer;
    std::shared_ptr<ITileBuffer> m_tileBuffer;
    std::vector<bool> m_commandVisibleTable;
    std::vector<int32_t> m_commandIndexTable;
    CameraData m_cameraData;
    std::shared_ptr<Buffer> m_cameraBuffer;
    TransformData m_transformData;
    std::shared_ptr<Buffer> m_transformBuffer;
    ColorData m_colorData;
    std::shared_ptr<Buffer> m_colorBuffer;
    int32_t m_indexLength;
    bool m_shouldCompact;
    bool m_shouldCommandCopy;
    bool m_shouldConstantCopy;
    bool m_shouldCameraCopy;

#if SOLID_ENABLE_INTERNAL
    struct IndirectCommand {
    public:
        explicit IndirectCommand() = default;
        D3D12_GPU_VIRTUAL_ADDRESS instanceBuffer;
        D3D12_GPU_VIRTUAL_ADDRESS cameraBuffer;
        D3D12_GPU_VIRTUAL_ADDRESS transformBuffer;
        D3D12_GPU_VIRTUAL_ADDRESS colorBuffer;
        D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
    };

    std::vector<IndirectCommand> m_commands;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12CommandSignature> m_commandSignature;
#endif
};
}