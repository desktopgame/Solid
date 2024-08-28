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

    Lib::Graphics::Renderer renderer;
    window->show();
    while (window->peekMessage()) {
        Lib::Input::Gamepad::sync();

        surface->begin();
        renderer.drawPlane(Vector3({ 0, 0, 10 }), Vector3({ 1, 1, 1 }), Color({ 0.5f, 0.0f, 0.0f, 1.0f }));

        renderer.drawRect(Vector2({ 400, 0 }), Vector2({ 100, 100 }), Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawRect(Vector2({ -400, 0 }), Vector2({ 100, 100 }), Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        renderer.drawCircle(Vector2({ 200, 200 }), Vector2({ 100, 100 }), Color({ 1.0f, 0.0f, 0.0f, 1.0f }));
        surface->end();

        Constant::release();

        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}