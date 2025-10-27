#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace App::Common::Graphics {
class Node;
/**
 * ノードのレジストリです。
 */
class NodeRegistry {
public:
    static void initialize();
    static void destroy();

    static std::shared_ptr<Node> s_slimeNode;
    static std::shared_ptr<Node> s_bulletNode;

private:
    NodeRegistry() = delete;
};
}