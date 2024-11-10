#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace Lib::Scene {
class IScene;
class SceneManager {
public:
    explicit SceneManager(
        const std::unordered_map<std::string, std::shared_ptr<IScene>>& sceneMap,
        const std::string& firstScene);

    void onUpdate();
    void onGui();
    void onDraw3D();
    void onDraw2D();

private:
    std::unordered_map<std::string, std::shared_ptr<IScene>> m_sceneMap;
    std::string m_currentScene;
    bool m_fire;
};
}