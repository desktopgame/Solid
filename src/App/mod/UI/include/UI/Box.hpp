#pragma once
#include <UI/Component.hpp>

namespace Lib::UI {
class Box {
public:
    static std::shared_ptr<Container> createHorizontalBox();
    static std::shared_ptr<Container> createVerticalBox();

private:
    Box() = delete;
    ~Box() = delete;
};
}