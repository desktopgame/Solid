#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

// App
#include <Debug/DebugScene.hpp>

// Lib
#include <library.hpp>

static int appMain(int argc, char* argv[])
{
    Screen::setSize(800, 600);

    auto engine = Engine::getInstance()->startup(argc, argv);
    auto window = engine->getWindow();

    auto inputSystem = InputSystem::getInstance()->startup();

    std::unordered_map<std::string, std::shared_ptr<IScene>> sceneMap;
    sceneMap.insert_or_assign("Debug", std::make_shared<App::Debug::DebugScene>());
    SceneManager sceneManager(sceneMap, "Debug");

    Renderer renderer;
    window->show();
    while (window->peekMessage()) {
        inputSystem->sync();

        sceneManager.onUpdate();

        renderer.guiBegin();
        sceneManager.onGui();
        renderer.guiEnd();

        renderer.begin();
        sceneManager.onDraw(renderer);
        renderer.end();
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