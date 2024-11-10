#pragma once
#include <memory>
#include <string>

namespace Lib::Scene {
class IScene {
public:
    virtual ~IScene() = 0;

    virtual void onEnter() = 0;
    virtual void onExit() = 0;

    virtual void onUpdate() = 0;
    virtual void onGui() = 0;
    virtual void onDraw3D() = 0;
    virtual void onDraw2D() = 0;

    virtual bool tryTransition(std::string& outNextScene) = 0;

private:
};
}