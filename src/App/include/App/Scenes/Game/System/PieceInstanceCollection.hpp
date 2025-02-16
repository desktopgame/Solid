#pragma once
#include <App/Scenes/Game/System/PieceInstance.hpp>
#include <memory>
#include <vector>

namespace App::Scenes::Game::System {
class PieceInstanceCollection {
public:
    explicit PieceInstanceCollection();

    void addInstance(const std::shared_ptr<PieceInstance>& instance);
    std::shared_ptr<PieceInstance> getInstanceAt(int32_t index) const;
    void removeInstanceAt(int32_t index);
    int32_t getInstanceCount() const;

private:
    std::vector<std::shared_ptr<PieceInstance>> m_instances;
};
}