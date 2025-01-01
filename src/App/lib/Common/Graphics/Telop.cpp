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

void Telop::initialize()
{
    m_elapsed = 0.0f;
}

void Telop::update()
{
    if (!isExpired()) {
        m_elapsed += Time::deltaTime();
    }
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