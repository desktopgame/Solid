#pragma once
#include <App/Scenes/Game/System/PieceInfo.hpp>
#include <App/Scenes/Game/UI/Piece.hpp>
#include <App/library.hpp>

namespace App::Scenes::Game::UI {
class PieceInfoListCellRenderer : public Panel, public IListCellRenderer<System::PieceInfo> {
public:
    static std::shared_ptr<PieceInfoListCellRenderer> create(const std::shared_ptr<FontMap>& fontMap, int32_t fontSize, const Vector2& labelPreferredSize);

    std::shared_ptr<Component> getListCellRendererComponent(const std::shared_ptr<List<System::PieceInfo>>& list, const System::PieceInfo& item, int32_t index, bool isSelected) override;

private:
    PieceInfoListCellRenderer();
    void init(const std::shared_ptr<FontMap>& fontMap, int32_t fontSize, const Vector2& labelPreferredSize);

    std::shared_ptr<Piece> m_piece;
    std::shared_ptr<Label> m_label;
};
}