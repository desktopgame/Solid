#pragma once
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <App/library.hpp>
#include <memory>

namespace App::Scenes::Game::UI {
/**
 * 周囲のフィールドと存在するエンティティを描画するミニマップです。
 */
class Minimap : public Component {
public:
    static inline const float k_routeLength = 5;

    explicit Minimap(const std::shared_ptr<System::Field>& field);

    void update() override;
    void draw2D(const std::shared_ptr<Renderer>& renderer) override;

private:
    bool wasGotCell(int32_t x, int32_t y) const;

    std::shared_ptr<System::Field> m_field;
};
}