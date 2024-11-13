#pragma once
#include <library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Debug {
class DebugScene : public IScene {
public:
    explicit DebugScene();
    virtual ~DebugScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    Vector3 m_cameraPos;
    Vector3 m_cameraLookAt;
    float m_cameraAngleX;
    float m_cameraAngleY;
    float m_cameraMoveSpeed;
    float m_cameraRotateSpeed;
    bool m_cursorVisible;
    bool m_editCompleted;

    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<Texture> m_debugTexture;

    std::shared_ptr<Buffer> m_vertexBuffer;
    std::shared_ptr<Buffer> m_indexBuffer;
    Reflect::UTileTransform m_tileTransform;
    Reflect::UTilePallet m_tilePallet;
    std::vector<std::shared_ptr<Buffer>> m_instanceBuffers;
    int32_t m_indexLength;
};
}