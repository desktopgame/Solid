#include <Scene/IScene.hpp>
#include <Scene/SceneManager.hpp>

namespace Lib::Scene {
// public
SceneManager::SceneManager(
    const std::unordered_map<std::string, std::shared_ptr<IScene>>& sceneMap,
    const std::string& firstScene)
    : m_sceneMap(sceneMap)
    , m_currentScene(firstScene)
    , m_fire()
{
}

void SceneManager::onUpdate(Graphics::Renderer& renderer)
{
    if (!m_fire) {
        m_fire = true;
        m_sceneMap[m_currentScene]->onEnter();
    }
    std::string nextScene;
    m_sceneMap[m_currentScene]->onUpdate(renderer);
    if (m_sceneMap[m_currentScene]->tryTransition(nextScene)) {
        m_sceneMap[m_currentScene]->onExit();

        m_currentScene = nextScene;
        m_sceneMap[m_currentScene]->onEnter();
    }
}

void SceneManager::onGui(Graphics::Renderer& renderer)
{
#if _DEBUG
    m_sceneMap[m_currentScene]->onGui(renderer);
#endif
}

void SceneManager::onDraw(Graphics::Renderer& renderer)
{
    m_sceneMap[m_currentScene]->onDraw(renderer);
}
}
