#pragma once
#include <App/Common/Graphics/Node.hpp>
#include <App/Common/Util/BackButton.hpp>
#include <App/Common/Util/FpsController.hpp>
#include <App/library.hpp>
#include <memory>
#include <optional>

namespace App::Scenes::Demo2D {
class Demo2DScene : public IScene {
public:
    explicit Demo2DScene();
    virtual ~Demo2DScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw3D() override;
    void onDraw2D() override;

    bool tryTransition(std::string& outNextScene) override;

private:
    Vector2 m_spritePos;
    Vector2 m_spriteSize;
    Vector4 m_spriteColor;

    Vector2 m_rectPos;
    Vector2 m_rectSize;
    Vector4 m_rectColor;

    Vector2 m_circlePos;
    Vector2 m_circleSize;
    Vector4 m_circleColor;

    std::shared_ptr<Texture> m_sampleTexture;
    std::string m_nextScene;
    std::shared_ptr<Renderer> m_renderer;

    std::unique_ptr<App::Common::Util::BackButton> m_backButton;
};
}