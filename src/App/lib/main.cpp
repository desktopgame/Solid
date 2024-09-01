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
    auto texture = Lib::Graphics::Texture::create(L"assets/Sprite.png");

    float degree = 0.0f;
    Lib::Graphics::Renderer renderer;
    window->show();
    while (window->peekMessage()) {
        Lib::Input::Gamepad::sync();

        Quaternion rotation = Quaternion::angleAxis(degree, Vector3({ 1, 0, 0 }));

        surface->begin();
        renderer.lightEnable();
        renderer.drawBox(Vector3({ 2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.lightDisable();
        renderer.drawBox(Vector3({ -2, 0, 2 }), Vector3({ 1, 1, 1 }), rotation, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));

        renderer.drawRect(Vector2({ 400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawRect(Vector2({ -400, 0 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawCircle(Vector2({ 200, 200 }), Vector2({ 100, 100 }), degree, Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawSprite(Vector2({ -300, 200 }), Vector2({ 100, 100 }), degree, texture, Color({ 1.0f, 1.0f, 1.0f, 1.0f }));
        degree += 1.0f;
        surface->end();

        Constant::release();

        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}