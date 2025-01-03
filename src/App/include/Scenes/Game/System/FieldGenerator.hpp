#pragma once
#include <library.hpp>
#include <vector>

namespace App::Scenes::Game::System {
class FieldGenerator {
public:
    explicit FieldGenerator();
    void generate();

    const std::vector<Vector4>& getTiles() const;

private:
    class Room {
    public:
        explicit Room();
        bool isContains(const IntVector3& at) const;
        bool isOverwrap(const Room& other) const;
        IntVector3 size;
        IntVector3 center;
        int32_t index;
        int32_t linkTo;
        bool isGarbage;
        bool isNoLink;
        bool isGenFailed;
    };

    std::vector<Vector4> m_tiles;
};
}