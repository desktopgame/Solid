#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

// App
#include <Common/Constants.hpp>
#include <Scenes/Debug/DebugScene.hpp>
#include <Scenes/Game/GameScene.hpp>
using namespace App::Common;
using namespace App::Scenes;

// Lib
#include <library.hpp>

static int appMain(int argc, char* argv[])
{
    Screen::setSize(800, 600);

    auto engine = Engine::getInstance()->startup(argc, argv);
    auto window = engine->getWindow();
    auto surface = engine->getDevice()->getSurface();

    auto inputSystem = InputSystem::getInstance()->startup(window);

    std::unordered_map<std::string, std::shared_ptr<IScene>> sceneMap;
    sceneMap.insert_or_assign("Game", std::make_shared<Game::GameScene>());
    sceneMap.insert_or_assign("Debug", std::make_shared<Debug::DebugScene>());
    SceneManager sceneManager(sceneMap, "Debug");

    Renderer renderer(Constants::k_tileSize);
    window->show();
    while (window->peekMessage()) {
        inputSystem->sync();

        sceneManager.onUpdate();

        surface->guiBegin();
        sceneManager.onGui();
        surface->guiEnd();

        surface->begin();
        sceneManager.onDraw();
        surface->end();
    }
    inputSystem->shutdown();
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