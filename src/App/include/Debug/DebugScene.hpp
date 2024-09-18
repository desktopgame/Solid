#pragma once
#include <library.hpp>

namespace App::Debug {
class DebugScene : public IScene {
public:
    explicit DebugScene();
    virtual ~DebugScene();

    void onEnter() override;
    void onExit() override;

    void onUpdate() override;
    void onGui() override;
    void onDraw(Renderer& renderer) override;

    bool tryTransition(std::string& outNextScene) override;

private:
};
}