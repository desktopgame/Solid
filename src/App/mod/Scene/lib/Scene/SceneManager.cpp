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

void SceneManager::onUpdate()
{
    if (!m_fire) {
        m_fire = true;
        m_sceneMap[m_currentScene]->onEnter();
    }
    std::string nextScene;
    m_sceneMap[m_currentScene]->onUpdate();
    if (m_sceneMap[m_currentScene]->tryTransition(nextScene)) {
        m_sceneMap[m_currentScene]->onExit();

        m_currentScene = nextScene;
        m_sceneMap[m_currentScene]->onEnter();
    }
}

void SceneManager::onGui()
{
#if _DEBUG
    m_sceneMap[m_currentScene]->onGui();
#endif
}

void SceneManager::onDraw3D()
{
    m_sceneMap[m_currentScene]->onDraw3D();
}

void SceneManager::onDraw2D()
{
    m_sceneMap[m_currentScene]->onDraw2D();
}
}
