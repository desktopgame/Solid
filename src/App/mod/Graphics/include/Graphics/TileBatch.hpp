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
    inline static const float s_tileSize = 1.0f;

    inline static const float s_tileHalf = s_tileSize / 2.0f;

    inline static const std::array<Math::Matrix, 6> s_translateMatrixTable = {
        // posY
        Math::Matrix::translate(Math::Vector3({ 0.0f, s_tileHalf, 0.0f })),
        // negY
        Math::Matrix::translate(Math::Vector3({ 0.0f, -s_tileHalf, 0.0f })),
        // posX
        Math::Matrix::translate(Math::Vector3({ s_tileHalf, 0.0f, 0.0f })),
        // negX
        Math::Matrix::translate(Math::Vector3({ -s_tileHalf, 0.0f, 0.0f })),
        // posZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, s_tileHalf })),
        // negZ
        Math::Matrix::translate(Math::Vector3({ 0.0f, 0.0f, -s_tileHalf })),
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

    inline static const std::array<Math::Vector4, 64> s_colorTable = {
        // https://lospec.com/palette-list/tag/64
        // Jehkoba64
        Math::Vector4({ 0.0f, 0.0f, 0.0f, 1.0f }),
        Math::Vector4({ 0.9803921568627451f, 0.7333333333333333f, 0.6862745098039216f, 1.0f }),
        Math::Vector4({ 0.9215686274509803f, 0.4588235294117647f, 0.5607843137254902f, 1.0f }),
        Math::Vector4({ 0.8509803921568627f, 0.2980392156862745f, 0.5568627450980392f, 1.0f }),
        Math::Vector4({ 0.7019607843137254f, 0.17647058823529413f, 0.49019607843137253f, 1.0f }),
        Math::Vector4({ 0.9803921568627451f, 0.596078431372549f, 0.5686274509803921f, 1.0f }),
        Math::Vector4({ 1.0f, 0.4392156862745098f, 0.4392156862745098f, 1.0f }),
        Math::Vector4({ 0.9607843137254902f, 0.19215686274509805f, 0.2549019607843137f, 1.0f }),
        Math::Vector4({ 0.7686274509803922f, 0.047058823529411764f, 0.1803921568627451f, 1.0f }),
        Math::Vector4({ 0.5215686274509804f, 0.13333333333333333f, 0.39215686274509803f, 1.0f }),
        Math::Vector4({ 0.9803921568627451f, 0.6274509803921569f, 0.19607843137254902f, 1.0f }),
        Math::Vector4({ 0.9607843137254902f, 0.5058823529411764f, 0.13333333333333333f, 1.0f }),
        Math::Vector4({ 0.9490196078431372f, 0.3843137254901961f, 0.12156862745098039f, 1.0f }),
        Math::Vector4({ 0.8588235294117647f, 0.29411764705882354f, 0.08627450980392157f, 1.0f }),
        Math::Vector4({ 0.6196078431372549f, 0.2980392156862745f, 0.2980392156862745f, 1.0f }),
        Math::Vector4({ 0.9803921568627451f, 0.8509803921568627f, 0.21568627450980393f, 1.0f }),
        Math::Vector4({ 1.0f, 0.7254901960784313f, 0.2196078431372549f, 1.0f }),
        Math::Vector4({ 0.9019607843137255f, 0.6078431372549019f, 0.13333333333333333f, 1.0f }),
        Math::Vector4({ 0.8f, 0.5019607843137255f, 0.1607843137254902f, 1.0f }),
        Math::Vector4({ 0.6784313725490196f, 0.41568627450980394f, 0.27058823529411763f, 1.0f }),
        Math::Vector4({ 0.8f, 0.7803921568627451f, 0.23921568627450981f, 1.0f }),
        Math::Vector4({ 0.7019607843137254f, 0.6901960784313725f, 0.17647058823529413f, 1.0f }),
        Math::Vector4({ 0.596078431372549f, 0.611764705882353f, 0.15294117647058825f, 1.0f }),
        Math::Vector4({ 0.5490196078431373f, 0.5019607843137255f, 0.1411764705882353f, 1.0f }),
        Math::Vector4({ 0.47843137254901963f, 0.3686274509803922f, 0.21568627450980393f, 1.0f }),
        Math::Vector4({ 0.5803921568627451f, 0.7490196078431373f, 0.18823529411764706f, 1.0f }),
        Math::Vector4({ 0.3333333333333333f, 0.7019607843137254f, 0.23137254901960785f, 1.0f }),
        Math::Vector4({ 0.09019607843137255f, 0.611764705882353f, 0.2627450980392157f, 1.0f }),
        Math::Vector4({ 0.023529411764705882f, 0.5019607843137255f, 0.3176470588235294f, 1.0f }),
        Math::Vector4({ 0.06666666666666667f, 0.3764705882352941f, 0.3803921568627451f, 1.0f }),
        Math::Vector4({ 0.6274509803921569f, 0.9215686274509803f, 0.6588235294117647f, 1.0f }),
        Math::Vector4({ 0.48627450980392156f, 0.8117647058823529f, 0.6039215686274509f, 1.0f }),
        Math::Vector4({ 0.3607843137254902f, 0.7215686274509804f, 0.5333333333333333f, 1.0f }),
        Math::Vector4({ 0.23921568627450981f, 0.6313725490196078f, 0.49411764705882355f, 1.0f }),
        Math::Vector4({ 0.12549019607843137f, 0.5019607843137255f, 0.4235294117647059f, 1.0f }),
        Math::Vector4({ 0.28627450980392155f, 0.7607843137254902f, 0.9490196078431372f, 1.0f }),
        Math::Vector4({ 0.1450980392156863f, 0.6745098039215687f, 0.9607843137254902f, 1.0f }),
        Math::Vector4({ 0.09019607843137255f, 0.5764705882352941f, 0.9019607843137255f, 1.0f }),
        Math::Vector4({ 0.10980392156862745f, 0.4588235294117647f, 0.7411764705882353f, 1.0f }),
        Math::Vector4({ 0.09803921568627451f, 0.3568627450980392f, 0.6509803921568628f, 1.0f }),
        Math::Vector4({ 0.6823529411764706f, 0.5333333333333333f, 0.8901960784313725f, 1.0f }),
        Math::Vector4({ 0.49411764705882355f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
        Math::Vector4({ 0.34509803921568627f, 0.41568627450980394f, 0.7686274509803922f, 1.0f }),
        Math::Vector4({ 0.20784313725490197f, 0.3254901960784314f, 0.6509803921568628f, 1.0f }),
        Math::Vector4({ 0.1411764705882353f, 0.2235294117647059f, 0.4f, 1.0f }),
        Math::Vector4({ 0.8862745098039215f, 0.6078431372549019f, 0.9803921568627451f, 1.0f }),
        Math::Vector4({ 0.792156862745098f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
        Math::Vector4({ 0.6392156862745098f, 0.36470588235294116f, 0.8509803921568627f, 1.0f }),
        Math::Vector4({ 0.4666666666666667f, 0.23137254901960785f, 0.7490196078431373f, 1.0f }),
        Math::Vector4({ 0.3058823529411765f, 0.15294117647058825f, 0.5490196078431373f, 1.0f }),
        Math::Vector4({ 0.7098039215686275f, 0.5490196078431373f, 0.4980392156862745f, 1.0f }),
        Math::Vector4({ 0.6196078431372549f, 0.4666666666666667f, 0.403921568627451f, 1.0f }),
        Math::Vector4({ 0.5294117647058824f, 0.36470588235294116f, 0.34509803921568627f, 1.0f }),
        Math::Vector4({ 0.43137254901960786f, 0.25882352941176473f, 0.3137254901960784f, 1.0f }),
        Math::Vector4({ 0.2784313725490196f, 0.1803921568627451f, 0.24313725490196078f, 1.0f }),
        Math::Vector4({ 0.6509803921568628f, 0.6039215686274509f, 0.611764705882353f, 1.0f }),
        Math::Vector4({ 0.5019607843137255f, 0.4745098039215686f, 0.5019607843137255f, 1.0f }),
        Math::Vector4({ 0.4117647058823529f, 0.396078431372549f, 0.4392156862745098f, 1.0f }),
        Math::Vector4({ 0.28627450980392155f, 0.3176470588235294f, 0.4117647058823529f, 1.0f }),
        Math::Vector4({ 0.050980392156862744f, 0.12941176470588237f, 0.25098039215686274f, 1.0f }),
        Math::Vector4({ 0.0196078431372549f, 0.054901960784313725f, 0.10196078431372549f, 1.0f }),
        Math::Vector4({ 0.8509803921568627f, 0.6549019607843137f, 0.596078431372549f, 1.0f }),
        Math::Vector4({ 0.7686274509803922f, 0.7333333333333333f, 0.7019607843137254f, 1.0f }),
        Math::Vector4({ 0.9490196078431372f, 0.9490196078431372f, 0.8549019607843137f, 1.0f }),
    };

private:
    TileBatch();

    struct CameraData {
    public:
        explicit CameraData() = default;
        Math::Matrix viewMatrix;
        Math::Matrix projectionMatrix;
        std::array<Math::Vector4, 8> padding;
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
        std::array<Math::Vector4, 64> colorTable;
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