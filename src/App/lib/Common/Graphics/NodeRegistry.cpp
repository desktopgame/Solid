#include <Common/Graphics/Node.hpp>
#include <Common/Graphics/NodeRegistry.hpp>

namespace App::Common::Graphics {
std::shared_ptr<Node> NodeRegistry::s_slimeNode;

void NodeRegistry::initialize()
{
    s_slimeNode = Common::Graphics::Node::deserialize("./assets/Models/sample.json");
}
void NodeRegistry::destroy()
{
    s_slimeNode = nullptr;
}
}