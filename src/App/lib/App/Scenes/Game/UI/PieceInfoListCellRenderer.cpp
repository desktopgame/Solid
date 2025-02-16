#include <App/Scenes/Game/UI/PieceInfoListCellRenderer.hpp>

namespace App::Scenes::Game::UI {
// public
std::shared_ptr<PieceInfoListCellRenderer> PieceInfoListCellRenderer::create(const std::shared_ptr<FontMap>& fontMap, int32_t fontSize, const Vector2& labelPreferredSize)
{
    auto cellRenderer = std::shared_ptr<PieceInfoListCellRenderer>(new PieceInfoListCellRenderer());
    cellRenderer->init(fontMap, fontSize, labelPreferredSize);
    return cellRenderer;
}

std::shared_ptr<Component> PieceInfoListCellRenderer::getListCellRendererComponent(const std::shared_ptr<List<std::shared_ptr<System::PieceInfo>>>& list, const std::shared_ptr<System::PieceInfo>& item, int32_t index, bool isSelected)
{
    auto self = shared_from_this();
    m_piece->setCells(item->cells);
    m_label->setText(item->text);
    if (isSelected) {
        setBackgroundColor(list->getSelectColor());
    } else {
        setBackgroundColor(list->getUnselectColor());
    }
    auto size = getLayout()->computePreferredSize(std::static_pointer_cast<Container>(self));
    setSize(size);
    setPreferredSize(size);
    doLayout();
    return self;
}
// private
PieceInfoListCellRenderer::PieceInfoListCellRenderer()
    : Panel()
    , m_piece()
    , m_label()
{
}

void PieceInfoListCellRenderer::init(const std::shared_ptr<FontMap>& fontMap, int32_t fontSize, const Vector2& labelPreferredSize)
{
    setLayout(std::make_shared<BoxLayout>(BoxLayout::Orientation::Horizontal));
    m_piece = std::make_shared<Piece>();
    addLayoutElement(std::make_shared<LayoutElement>(m_piece, nullptr));
    m_label = std::make_shared<Label>();
    m_label->setFont(fontMap);
    m_label->setFontSize(fontSize);
    m_label->setPreferredSize(labelPreferredSize);
    addLayoutElement(std::make_shared<LayoutElement>(m_label, nullptr));
}
}