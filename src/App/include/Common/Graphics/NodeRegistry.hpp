#pragma once
#include <memory>
#include <string>
#include <unordered_map>


namespace App::Common::Graphics {
class Node;
class NodeRegistry {
public:
    static void initialize();
    static void destroy();

    static std::shared_ptr<Node> s_slimeNode;

private:
    NodeRegistry() = delete;
};
}