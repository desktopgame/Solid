#pragma once
#include <App/library.hpp>

namespace App::Common::Util {
class BackButton {
public:
    explicit BackButton() = default;
    ~BackButton() = default;
    void init(const std::shared_ptr<FontMap>& font, const std::function<void()>& onClick);
    void update();
    void draw(const std::shared_ptr<Renderer>& renderer);

private:
    std::shared_ptr<Button> m_button;
};
}