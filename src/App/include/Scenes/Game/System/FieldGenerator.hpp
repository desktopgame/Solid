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
        bool removed;
        bool connected;
    };

    static void markRecursive(int32_t index, const std::vector<Room>& rooms, std::vector<int32_t>& visit);
    static std::vector<int32_t> markRecursive(int32_t index, const std::vector<Room>& rooms);

    std::vector<Vector4> m_tiles;
};
}