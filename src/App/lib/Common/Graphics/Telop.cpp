#include <Common/Graphics/Telop.hpp>

namespace App::Common::Graphics {
// public
Telop::Telop()
    : text()
    , position()
    , color()
    , duration()
    , m_elapsed()
{
}

void Telop::update()
{
}

void Telop::draw()
{
}

void Telop::destroy()
{
}

bool Telop::isExpired() const { return m_elapsed >= duration; }
// private
}