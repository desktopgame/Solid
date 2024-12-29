#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/NodeRegistry.hpp>

namespace App::Common::Graphics {
std::shared_ptr<Node> NodeRegistry::s_slimeNode;
std::shared_ptr<Node> NodeRegistry::s_bulletNode;

void NodeRegistry::initialize()
{
    s_slimeNode = Common::Graphics::Node::deserialize("./assets/Models/Sample.json");
    s_bulletNode = Common::Graphics::Node::deserialize("./assets/Models/Bullet.json");
}
void NodeRegistry::destroy()
{
    s_slimeNode = nullptr;
    s_bulletNode = nullptr;
}
}