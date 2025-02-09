#pragma once
#include <UI/Container.hpp>

namespace Lib::UI {
class Panel : public Container {
public:
    explicit Panel();
    virtual ~Panel();

    virtual void draw2D(const std::shared_ptr<Graphics::Renderer>& renderer) override;

private:
};
}