#include <Graphics/Device.hpp>
#include <Graphics/Engine.hpp>
#include <Graphics/Window.hpp>

int main(int argc, char* argv[])
{
    auto engine = Lib::Graphics::Engine::getInstance()->startup(argc, argv);
    auto device = engine->getDevice();
    auto window = engine->getWindow();
    window->show();
    while (true) {
        if (window->translateMessage()) {
            break;
        }
        device->render();
        // Show messages
        device->flushLogEntries();
    }
    engine->shutdown();
    return 0;
}