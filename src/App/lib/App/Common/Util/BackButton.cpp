#include <App/Common/Util/BackButton.hpp>

namespace App::Common::Util {
void BackButton::init(const std::shared_ptr<FontMap>& font, const std::function<void()>& onClick)
{
    m_button = std::make_shared<Button>();
    m_button->setFont(font);
    m_button->setText(u"BACK");
    m_button->setSize(Vector2({ 80, 40 }));
    m_button->setPosition(Vector2({ 0,
        -(Screen::getHeight() / 2.0f) + 20 }));
    m_button->setOnClick(onClick);
}

void BackButton::update()
{
    m_button->update();
}

void BackButton::draw(const std::shared_ptr<Renderer>& renderer)
{
    m_button->draw2D(renderer);
}
}