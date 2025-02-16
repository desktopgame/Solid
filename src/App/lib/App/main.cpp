#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

// App
#include <App/Common/Graphics/NodeRegistry.hpp>
#include <App/Common/Graphics/ParticleSystem.hpp>
#include <App/Common/Graphics/TelopSystem.hpp>
#include <App/Scenes/Debug/DebugScene.hpp>
#include <App/Scenes/Demo/DemoScene.hpp>
#include <App/Scenes/Game/GameScene.hpp>
#include <App/Scenes/Game/System/PieceRegistry.hpp>
#include <App/Scenes/Title/TitleScene.hpp>
#include <App/test.hpp>
using namespace App::Common;
using namespace App::Scenes;

// Lib
#include <App/library.hpp>

static int appMain(int argc, char* argv[])
{
    Screen::setSize(800, 600);

    auto engine = Engine::getInstance()->startup(argc, argv);
    auto window = engine->getWindow();
    auto surface = engine->getDevice()->getSurface();

    auto inputSystem = InputSystem::getInstance()->startup(window);

    std::unordered_map<std::string, std::shared_ptr<IScene>> sceneMap;
    sceneMap.insert_or_assign("Title", std::make_shared<Title::TitleScene>());
    sceneMap.insert_or_assign("Game", std::make_shared<Game::GameScene>());
    sceneMap.insert_or_assign("Demo", std::make_shared<Demo::DemoScene>());
    sceneMap.insert_or_assign("Debug", std::make_shared<Debug::DebugScene>());
    std::unique_ptr<SceneManager> sceneManager = std::make_unique<SceneManager>(sceneMap, "Debug");

    Graphics::NodeRegistry::initialize();
    Graphics::ParticleSystem::initialize();
    Graphics::TelopSystem::initialize();
    Game::System::PieceRegistry::initialize();

    window->show();
    bool running = true;
    while (running) {
        Time::begin();
        running = window->peekMessage();
        inputSystem->sync();

        surface->beginPresent();
        sceneManager->onUpdate();
        surface->endPresent();

        surface->beginGui();
        sceneManager->onGui();
        surface->endGui();

        surface->begin3D();
        sceneManager->onDraw3D();
        surface->end3D();

        surface->begin2D();
        sceneManager->onDraw2D();
        surface->end2D();
        Time::end();
        Time::sync();
    }

    Game::System::PieceRegistry::destroy();
    Graphics::TelopSystem::destroy();
    Graphics::ParticleSystem::destroy();
    Graphics::NodeRegistry::destroy();

    sceneManager = nullptr;
    sceneMap.clear();

    inputSystem->shutdown();
    engine->shutdown();
    return 0;
}

#ifdef _TEST
static int appTest(int argc, char* argv[])
{
    auto engine = Engine::getInstance()->startup(argc, argv);
    auto inputSystem = InputSystem::getInstance()->startup(engine->getWindow());
    Graphics::NodeRegistry::initialize();
    runAllTests();
    Graphics::NodeRegistry::destroy();
    inputSystem->shutdown();
    engine->shutdown();
    return 0;
}
#endif

int main(int argc, char* argv[])
{
#if _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CrtSetBreakAlloc(1441);
#endif
#ifdef _TEST
    return appTest(argc, argv);
#else
    return appMain(argc, argv);
#endif
}