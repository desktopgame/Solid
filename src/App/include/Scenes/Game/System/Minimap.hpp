#pragma once
#include <Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <Scenes/Game/System/Entity.hpp>
#include <Scenes/Game/System/Field.hpp>
#include <library.hpp>
#include <memory>


namespace App::Scenes::Game::System {
class Minimap {
public:
    explicit Minimap(const std::shared_ptr<Field>& field);

    void update();
    void draw2D(const std::shared_ptr<Renderer>& renderer);

private:
    void drawEntity(
        const std::shared_ptr<Renderer>& renderer,
        const std::shared_ptr<Entity>& entity,
        const std::shared_ptr<Chunk>& chunk,
        const Color& color) const;

    std::weak_ptr<Field> m_field;
    Vector2 m_position; // ミニマップの表示位置
    Vector2 m_size; // ミニマップのサイズ
};
}