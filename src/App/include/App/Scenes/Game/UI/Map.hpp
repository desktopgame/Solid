#pragma once
#include <App/Scenes/Game/System/Entities/PlayerEntity.hpp>
#include <App/Scenes/Game/System/Entity.hpp>
#include <App/Scenes/Game/System/Field.hpp>
#include <UI/Component.hpp>

namespace App::Scenes::Game::System {
class PieceInstanceCollection;
}
namespace App::Scenes::Game::UI {
class Map : public Component {
public:
    explicit Map(const std::shared_ptr<System::Field>& field);

    void setup();

    void update() override;
    void draw2D(const std::shared_ptr<Renderer>& renderer) override;

    void setPieceInstanceCollection(const std::shared_ptr<System::PieceInstanceCollection>& pieceInstanceCollection);
    std::shared_ptr<System::PieceInstanceCollection> getPieceInstanceCollection() const;

private:
    static inline constexpr float k_chunkWidth = 50;
    static inline constexpr float k_chunkHeight = 50;
    static inline constexpr float k_routeSize = 5;

    std::shared_ptr<System::Field> m_field;
    int32_t m_minChunkX;
    int32_t m_maxChunkX;
    int32_t m_minChunkY;
    int32_t m_maxChunkY;
    int32_t m_chunkCountX;
    int32_t m_chunkCountY;

    std::shared_ptr<System::PieceInstanceCollection> m_pieceInstanceCollection;
};
}