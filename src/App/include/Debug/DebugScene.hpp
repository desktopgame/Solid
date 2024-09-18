#pragma once
#include <library.hpp>

namespace App::Debug {
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
    Vector3 m_tilePosition;
    int32_t m_tileSide;
    int32_t m_tileColor;
    std::vector<Vector4> m_tileData;

    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;

    bool m_tileSubmit;
    int32_t m_tileID;
    int32_t m_previewTileID;
};
}