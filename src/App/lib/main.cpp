#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

#include <Graphics.hpp>
#include <Input.hpp>
#include <Math.hpp>
#include <cassert>
#include <fstream>
#include <imgui.h>
#include <picojson/picojson.h>

using namespace Lib::Graphics;
using namespace Lib::Math;

static int appMain(int argc, char* argv[])
{
    Screen::setSize(800, 600);

    auto engine = Engine::getInstance()->startup(argc, argv);
    auto device = engine->getDevice();
    auto window = engine->getWindow();
    auto surface = device->getSurface();
    auto texture = Lib::Graphics::Texture::create("assets/Sprite.png");
    auto texture2 = Lib::Graphics::Texture::create("assets/ghicon.png");

    auto fontMap = FontFactory::getInstance()->load("assets/NotoSansJP-Regular.ttf");
    fontMap->load(16, u"こんにちわ");

    float degree = 0.0f;
    bool isGuiOpen = true;
    Lib::Graphics::Renderer renderer;
    Lib::Math::Vector3 cameraPosition = Lib::Math::Vector3({ 0, 0, -1 });
    Lib::Math::Vector3 cameraLookAt = Lib::Math::Vector3({ 0, 0, 0 });
    window->show();
    while (window->peekMessage()) {
        Lib::Input::Gamepad::sync();

        Quaternion rotation = Quaternion::angleAxis(degree, Vector3({ 1, 0, 0 }));

        renderer.guiBegin();
        ImGui::Begin("Test", &isGuiOpen);
        ImGui::DragFloat("Degree", &degree, 1.0f, 0.0f, 360.0f);
        ImGui::DragFloat3("Position", cameraPosition.data());
        ImGui::DragFloat3("LookAt", cameraLookAt.data());
        ImGui::End();
        renderer.guiEnd();

        renderer.begin();
        renderer.position(cameraPosition);
        renderer.lookAt(cameraLookAt);
        renderer.lightEnable();
        renderer.drawBox(Vector3({ 2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.lightDisable();
        renderer.drawBox(Vector3({ -2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));

        renderer.drawRect(Vector2({ 400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawRect(Vector2({ -400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawCircle(Vector2({ 200, 200 }), Vector2({ 100, 100 }), Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawSprite(Vector2({ -300, 200 }), Vector2({ 100, 100 }), degree, texture, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
        renderer.drawSprite(Vector2({ 0, 200 }), Vector2({ 100, 100 }), degree, texture2, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
        renderer.drawTiles();

        renderer.textFont(fontMap);
        renderer.textFontSize(40);
        renderer.drawText(
            Vector2({ -(static_cast<float>(Screen::getWidth()) / 2.0f), (static_cast<float>(Screen::getHeight() / 2.0f)) }),
            Renderer::TextAlignX::Left,
            Renderer::TextAlignY::Top,
            u"Hello",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ (static_cast<float>(Screen::getWidth()) / 2.0f), (static_cast<float>(Screen::getHeight() / 2.0f)) }),
            Renderer::TextAlignX::Right,
            Renderer::TextAlignY::Top,
            u"こんにちわ",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ -(static_cast<float>(Screen::getWidth()) / 2.0f), (-static_cast<float>(Screen::getHeight() / 2.0f)) }),
            Renderer::TextAlignX::Left,
            Renderer::TextAlignY::Bottom,
            u"おはよう",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ (static_cast<float>(Screen::getWidth()) / 2.0f), (-static_cast<float>(Screen::getHeight() / 2.0f)) }),
            Renderer::TextAlignX::Right,
            Renderer::TextAlignY::Bottom,
            u"さようなら",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ 0, 0 }),
            Renderer::TextAlignX::Center,
            Renderer::TextAlignY::Center,
            u"Sample_Text",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ -(static_cast<float>(Screen::getWidth()) / 2.0f), 0 }),
            Renderer::TextAlignX::Center,
            Renderer::TextAlignY::Center,
            u"AAAA",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawText(
            Vector2({ (static_cast<float>(Screen::getWidth()) / 2.0f), 0 }),
            Renderer::TextAlignX::Center,
            Renderer::TextAlignY::Center,
            u"AAAA",
            Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        renderer.end();

        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}

int main(int argc, char* argv[])
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CrtSetBreakAlloc(1441);
#endif
    int code = appMain(argc, argv);
#if _DEBUG
    _CrtDumpMemoryLeaks();
#endif
    return code;
}