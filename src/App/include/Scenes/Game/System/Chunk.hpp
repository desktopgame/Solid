#pragma once
#include <Scenes/Game/System/ChunkGenerator.hpp>
#include <library.hpp>
#include <memory>
#include <vector>

namespace App::Scenes::Game::System {
class Entity;
namespace Entities {
    class PlayerEntity;
}
class Field;
class Chunk : public std::enable_shared_from_this<Chunk> {
public:
    static inline constexpr int32_t k_fieldSizeX = 132;
    static inline constexpr int32_t k_fieldSizeY = 16;
    static inline constexpr int32_t k_fieldSizeZ = 132;
    static inline constexpr int32_t k_roomSpace = 4;
    static inline constexpr int32_t k_roomSizeX = (k_fieldSizeX - (k_roomSpace * 4)) / 3;
    static inline constexpr int32_t k_roomSizeZ = (k_fieldSizeZ - (k_roomSpace * 4)) / 3;
    static inline constexpr float k_tileSize = 5.0f;
    static inline constexpr float k_gravity = 9.8f * 5.0f;
    static_assert(k_roomSpace % 2 == 0);
    static_assert(k_roomSizeX % 2 == 0);
    static_assert(k_roomSizeZ % 2 == 0);

    static inline const std::array<Matrix, 6> k_translateMatrixTable = {
        // posY
        Matrix::translate(Vector3({ 0.0f, 0.5f, 0.0f })),
        // negY
        Matrix::translate(Vector3({ 0.0f, -0.5f, 0.0f })),
        // posX
        Matrix::translate(Vector3({ 0.5f, 0.0f, 0.0f })),
        // negX
        Matrix::translate(Vector3({ -0.5f, 0.0f, 0.0f })),
        // posZ
        Matrix::translate(Vector3({ 0.0f, 0.0f, 0.5f })),
        // negZ
        Matrix::translate(Vector3({ 0.0f, 0.0f, -0.5f })),
    };
    static inline const std::array<Matrix, 6> k_rotationMatrixTable = {
        // posY
        Matrix::rotateX(-90.0f),
        // negY
        Matrix::rotateX(90.0f),
        // posX
        Matrix::rotateY(90.0f),
        // negX
        Matrix::rotateY(-90.0f),
        // posZ
        Matrix::rotateY(180.0f),
        // negZ
        Matrix(),
    };
    static inline const std::array<Vector3, 6> k_normalVectorTable = {
        // posY
        Vector3({ 0, 1, 0 }),
        // negY
        Vector3({ 0, -1, 0 }),
        // posX
        Vector3({ 1, 0, 0 }),
        // negX
        Vector3({ -1, 0, 0 }),
        // posZ
        Vector3({ 0, 0, 1 }),
        // negZ
        Vector3({ 0, 0, -1 }),
    };
    static inline const std::array<Vector4, 64> k_palletTable = {
        // https://lospec.com/palette-list/tag/64
        // Jehkoba64
        Vector4({ 0.0f, 0.0f, 0.0f, 1.0f }),
        Vector4({ 0.9803921568627451f, 0.7333333333333333f, 0.6862745098039216f, 1.0f }),
        Vector4({ 0.9215686274509803f, 0.4588235294117647f, 0.5607843137254902f, 1.0f }),
        Vector4({ 0.8509803921568627f, 0.2980392156862745f, 0.5568627450980392f, 1.0f }),
        Vector4({ 0.7019607843137254f, 0.17647058823529413f, 0.49019607843137253f, 1.0f }),
        Vector4({ 0.9803921568627451f, 0.596078431372549f, 0.5686274509803921f, 1.0f }),
        Vector4({ 1.0f, 0.4392156862745098f, 0.4392156862745098f, 1.0f }),
        Vector4({ 0.9607843137254902f, 0.19215686274509805f, 0.2549019607843137f, 1.0f }),
        Vector4({ 0.7686274509803922f, 0.047058823529411764f, 0.1803921568627451f, 1.0f }),
        Vector4({ 0.5215686274509804f, 0.13333333333333333f, 0.39215686274509803f, 1.0f }),
        Vector4({ 0.9803921568627451f, 0.6274509803921569f, 0.19607843137254902f, 1.0f }),
        Vector4({ 0.9607843137254902f, 0.5058823529411764f, 0.13333333333333333f, 1.0f }),
        Vector4({ 0.9490196078431372f, 0.3843137254901961f, 0.12156862745098039f, 1.0f }),
        Vector4({ 0.8588235294117647f, 0.29411764705882354f, 0.08627450980392157f, 1.0f }),
        Vector4({ 0.6196078431372549f, 0.2980392156862745f, 0.2980392156862745f, 1.0f }),
        Vector4({ 0.9803921568627451f, 0.8509803921568627f, 0.21568627450980393f, 1.0f }),
        Vector4({ 1.0f, 0.7254901960784313f, 0.2196078431372549f, 1.0f }),
        Vector4({ 0.9019607843137255f, 0.6078431372549019f, 0.13333333333333333f, 1.0f }),
        Vector4({ 0.8f, 0.5019607843137255f, 0.1607843137254902f, 1.0f }),
        Vector4({ 0.6784313725490196f, 0.41568627450980394f, 0.27058823529411763f, 1.0f }),
        Vector4({ 0.8f, 0.7803921568627451f, 0.23921568627450981f, 1.0f }),
        Vector4({ 0.7019607843137254f, 0.6901960784313725f, 0.17647058823529413f, 1.0f }),
        Vector4({ 0.596078431372549f, 0.611764705882353f, 0.15294117647058825f, 1.0f }),
        Vector4({ 0.5490196078431373f, 0.5019607843137255f, 0.1411764705882353f, 1.0f }),
        Vector4({ 0.47843137254901963f, 0.3686274509803922f, 0.21568627450980393f, 1.0f }),
        Vector4({ 0.5803921568627451f, 0.7490196078431373f, 0.18823529411764706f, 1.0f }),
        Vector4({ 0.3333333333333333f, 0.7019607843137254f, 0.23137254901960785f, 1.0f }),
        Vector4({ 0.09019607843137255f, 0.611764705882353f, 0.2627450980392157f, 1.0f }),
        Vector4({ 0.023529411764705882f, 0.5019607843137255f, 0.3176470588235294f, 1.0f }),
        Vector4({ 0.06666666666666667f, 0.3764705882352941f, 0.3803921568627451f, 1.0f }),
        Vector4({ 0.6274509803921569f, 0.9215686274509803f, 0.6588235294117647f, 1.0f }),
        Vector4({ 0.48627450980392156f, 0.8117647058823529f, 0.6039215686274509f, 1.0f }),
        Vector4({ 0.3607843137254902f, 0.7215686274509804f, 0.5333333333333333f, 1.0f }),
        Vector4({ 0.23921568627450981f, 0.6313725490196078f, 0.49411764705882355f, 1.0f }),
        Vector4({ 0.12549019607843137f, 0.5019607843137255f, 0.4235294117647059f, 1.0f }),
        Vector4({ 0.28627450980392155f, 0.7607843137254902f, 0.9490196078431372f, 1.0f }),
        Vector4({ 0.1450980392156863f, 0.6745098039215687f, 0.9607843137254902f, 1.0f }),
        Vector4({ 0.09019607843137255f, 0.5764705882352941f, 0.9019607843137255f, 1.0f }),
        Vector4({ 0.10980392156862745f, 0.4588235294117647f, 0.7411764705882353f, 1.0f }),
        Vector4({ 0.09803921568627451f, 0.3568627450980392f, 0.6509803921568628f, 1.0f }),
        Vector4({ 0.6823529411764706f, 0.5333333333333333f, 0.8901960784313725f, 1.0f }),
        Vector4({ 0.49411764705882355f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
        Vector4({ 0.34509803921568627f, 0.41568627450980394f, 0.7686274509803922f, 1.0f }),
        Vector4({ 0.20784313725490197f, 0.3254901960784314f, 0.6509803921568628f, 1.0f }),
        Vector4({ 0.1411764705882353f, 0.2235294117647059f, 0.4f, 1.0f }),
        Vector4({ 0.8862745098039215f, 0.6078431372549019f, 0.9803921568627451f, 1.0f }),
        Vector4({ 0.792156862745098f, 0.49411764705882355f, 0.9490196078431372f, 1.0f }),
        Vector4({ 0.6392156862745098f, 0.36470588235294116f, 0.8509803921568627f, 1.0f }),
        Vector4({ 0.4666666666666667f, 0.23137254901960785f, 0.7490196078431373f, 1.0f }),
        Vector4({ 0.3058823529411765f, 0.15294117647058825f, 0.5490196078431373f, 1.0f }),
        Vector4({ 0.7098039215686275f, 0.5490196078431373f, 0.4980392156862745f, 1.0f }),
        Vector4({ 0.6196078431372549f, 0.4666666666666667f, 0.403921568627451f, 1.0f }),
        Vector4({ 0.5294117647058824f, 0.36470588235294116f, 0.34509803921568627f, 1.0f }),
        Vector4({ 0.43137254901960786f, 0.25882352941176473f, 0.3137254901960784f, 1.0f }),
        Vector4({ 0.2784313725490196f, 0.1803921568627451f, 0.24313725490196078f, 1.0f }),
        Vector4({ 0.6509803921568628f, 0.6039215686274509f, 0.611764705882353f, 1.0f }),
        Vector4({ 0.5019607843137255f, 0.4745098039215686f, 0.5019607843137255f, 1.0f }),
        Vector4({ 0.4117647058823529f, 0.396078431372549f, 0.4392156862745098f, 1.0f }),
        Vector4({ 0.28627450980392155f, 0.3176470588235294f, 0.4117647058823529f, 1.0f }),
        Vector4({ 0.050980392156862744f, 0.12941176470588237f, 0.25098039215686274f, 1.0f }),
        Vector4({ 0.0196078431372549f, 0.054901960784313725f, 0.10196078431372549f, 1.0f }),
        Vector4({ 0.8509803921568627f, 0.6549019607843137f, 0.596078431372549f, 1.0f }),
        Vector4({ 0.7686274509803922f, 0.7333333333333333f, 0.7019607843137254f, 1.0f }),
        Vector4({ 0.9490196078431372f, 0.9490196078431372f, 0.8549019607843137f, 1.0f }),
    };

    explicit Chunk(
        std::weak_ptr<Field> field,
        const IntVector2& position,
        const std::shared_ptr<Texture>& normalTexture,
        const std::shared_ptr<Texture>& borderTexture);

    void generate();

    void update();
    void onGui();
    void draw3D(const std::shared_ptr<Renderer>& renderer);
    void draw2D(const std::shared_ptr<Renderer>& renderer);

    std::shared_ptr<Field> getField() const;

    inline IntVector2 getPosition() const
    {
        return m_position;
    }

    inline bool hasBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        if (x >= k_fieldSizeX || x < 0) {
            return false;
        }
        if (y >= k_fieldSizeY || y < 0) {
            return false;
        }
        if (z >= k_fieldSizeZ || z < 0) {
            return false;
        }
        return true;
    }

    inline int32_t getBlockAt(int32_t x, int32_t y, int32_t z) const
    {
        if (!hasBlockAt(x, y, z)) {
            return 1;
        }
        return m_blocks[toIndex(x, y, z)];
    }

    void spwan(const std::shared_ptr<Entity>& entity);
    std::shared_ptr<Entity> getEntityAt(int32_t index) const;
    int32_t getEntityCount() const;

    ChunkGenerator::Room getRoomAt(int32_t index) const;
    int32_t getRoomCount() const;

    static inline int32_t toIndex(int32_t x, int32_t y, int32_t z)
    {
        return (z * k_fieldSizeX * k_fieldSizeY) + (y * k_fieldSizeX) + x;
    }
    static inline IntVector3 toCoord(int32_t index)
    {
        const int z = index / (k_fieldSizeX * k_fieldSizeY);
        index -= (z * k_fieldSizeX * k_fieldSizeY);
        const int y = index / k_fieldSizeX;
        const int x = index % k_fieldSizeX;
        return IntVector3({ x, y, z });
    }

private:
    // NOTE: _ITERATOR_DEBUG_LEVELや_CONTAINER_DEBUG_LEVELの影響を受けるstd::arrayは使わない
    int32_t m_blocks[k_fieldSizeX * k_fieldSizeY * k_fieldSizeZ];
    std::vector<std::shared_ptr<Entity>> m_entities;

    std::shared_ptr<ChunkGenerator> m_generator;
    std::weak_ptr<Field> m_field;
    IntVector2 m_position;
    std::shared_ptr<Texture> m_normalTexture;
    std::shared_ptr<Texture> m_borderTexture;
    std::shared_ptr<CpuBuffer> m_vertexBuffer;
    std::shared_ptr<CpuBuffer> m_indexBuffer;
    Reflect::UTileTransform m_tileTransform;
    Reflect::UTilePallet m_tilePallet;
    std::vector<std::shared_ptr<IBuffer>> m_instanceBuffers;
    int32_t m_indexLength;
    int32_t m_instanceCount;

#if _DEBUG
    bool m_debugDrawChunk;
#endif
};
}