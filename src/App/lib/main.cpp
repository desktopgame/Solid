#include <Graphics.hpp>
#include <Input.hpp>
#include <Math.hpp>
#include <cassert>
#include <fstream>
#include <picojson/picojson.h>

using namespace Lib::Graphics;
using namespace Lib::Math;

int main(int argc, char* argv[])
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
    Lib::Graphics::Renderer renderer;
    window->show();
    while (window->peekMessage()) {
        Lib::Input::Gamepad::sync();

        Quaternion rotation = Quaternion::angleAxis(degree, Vector3({ 1, 0, 0 }));

        renderer.begin();
        renderer.lightEnable();
        renderer.drawBox(Vector3({ 2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.lightDisable();
        renderer.drawBox(Vector3({ -2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));

        renderer.drawRect(Vector2({ 400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawRect(Vector2({ -400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawCircle(Vector2({ 200, 200 }), Vector2({ 100, 100 }), Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawSprite(Vector2({ -300, 200 }), Vector2({ 100, 100 }), degree, texture, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
        renderer.drawSprite(Vector2({ 0, 200 }), Vector2({ 100, 100 }), degree, texture2, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));

        renderer.textFont(fontMap);
        renderer.textFontSize(40);
        renderer.drawText(Vector2({ 0, 0 }), 0.0f, u"こんにちわ世界", Color({ 0.0f, 0.0f, 0.0f, 1.0f }));
        degree += 1.0f;
        renderer.end();

        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}