#pragma once
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Debug {
class DebugScene : public IScene {
public:
    explicit DebugScene();
    virtual ~DebugScene();

    void onEnter(Renderer& renderer) override;
    void onExit(Renderer& renderer) override;

    void onUpdate(Renderer& renderer) override;
    void onGui(Renderer& renderer) override;
    void onDraw(Renderer& renderer) override;

    bool tryTransition(std::string& outNextScene) override;

private:
    int32_t getColorIndex() const;
    void compactTiles();
    void restoreTiles(const Vector3& center);
    std::optional<Vector3> scanHintTiles(Vector3 forward);

    Vector3 m_tilePosition;
    int32_t m_tileSide;
    int32_t m_tilePallet;
    int32_t m_tileColor;
    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;
    bool m_cursorVisible;
    std::array<char, 32> m_ioFile;

    int32_t m_tileID;
    std::vector<Vector4> m_tiles;

    int32_t m_hintTileID;
    std::vector<Vector4> m_hintTiles;
    std::vector<Vector4> m_placeTiles;
    std::shared_ptr<TileBatch> m_hintTileBatch;
};
}