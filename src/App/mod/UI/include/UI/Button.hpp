#pragma once
#include <UI/Component.hpp>
#include <functional>
#include <string>

namespace Lib::UI {
class Button : public Component {
public:
    explicit Button();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setText(const std::u16string& text);
    std::u16string getText() const;

    void setOnClick(const std::function<void()>& onClick);
    std::function<void()> getOnClick() const;

private:
    static inline constexpr int32_t k_buttonNone = 0;
    static inline constexpr int32_t k_buttonHover = 1;
    static inline constexpr int32_t k_buttonActive = 2;

    std::u16string m_text;
    int32_t m_status;
    std::function<void()> m_onClick;
};
}