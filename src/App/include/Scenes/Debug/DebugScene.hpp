#pragma once
#include <library.hpp>

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
};
}