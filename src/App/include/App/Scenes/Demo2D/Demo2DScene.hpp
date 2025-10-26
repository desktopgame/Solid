#pragma once
#include <App/Common/Graphics/Node.hpp>
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
    std::string m_nextScene;
    std::shared_ptr<Renderer> m_renderer;
};
}