#pragma once
#include <Graphics/Renderer.hpp>
#include <memory>
#include <string>

namespace Lib::Scene {
class IScene {
public:
    virtual ~IScene() = 0;

    virtual void onEnter(Graphics::Renderer& renderer) = 0;
    virtual void onExit(Graphics::Renderer& renderer) = 0;

    virtual void onUpdate(Graphics::Renderer& renderer) = 0;
    virtual void onGui(Graphics::Renderer& renderer) = 0;
    virtual void onDraw(Graphics::Renderer& renderer) = 0;

    virtual bool tryTransition(std::string& outNextScene) = 0;

private:
};
}