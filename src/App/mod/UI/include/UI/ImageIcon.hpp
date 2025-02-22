#pragma once
#include <Graphics/Color.hpp>
#include <Graphics/Texture.hpp>
#include <UI/Component.hpp>

namespace Lib::UI {
class ImageIcon : public Component {
public:
    explicit ImageIcon();

    void update() override;
    void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

    void setTexture(const std::shared_ptr<Graphics::Texture>& texture);
    std::shared_ptr<Graphics::Texture> getTexture() const;

private:
    std::shared_ptr<Graphics::Texture> m_texture;
};
}