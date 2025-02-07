#include <Scenes/Game/System/Minimap.hpp>

namespace App::Scenes::Game::System {
// public
Minimap::Minimap(const std::shared_ptr<Field>& field)
    : m_field(field)
{
}

void Minimap::update()
{
}

void Minimap::draw2D(const std::shared_ptr<Renderer>& renderer)
{
}
}