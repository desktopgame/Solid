#include <App/Scenes/Game/System/PieceInstanceCollection.hpp>

namespace App::Scenes::Game::System {

PieceInstanceCollection::PieceInstanceCollection()
    : m_instances()
{
}

void PieceInstanceCollection::addInstance(const std::shared_ptr<PieceInstance>& instance)
{
    m_instances.emplace_back(instance);
}
std::shared_ptr<PieceInstance> PieceInstanceCollection::getInstanceAt(int32_t index) const
{
    return m_instances.at(index);
}
void PieceInstanceCollection::removeInstanceAt(int32_t index)
{
    m_instances.erase(m_instances.begin() + index);
}
int32_t PieceInstanceCollection::getInstanceCount() const
{
    return static_cast<int32_t>(m_instances.size());
}
}