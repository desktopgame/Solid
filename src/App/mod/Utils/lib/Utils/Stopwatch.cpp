#include <Utils/Clock.hpp>
#include <Utils/Stopwatch.hpp>

namespace Lib::Utils {
// public
Stopwatch::Stopwatch()
{
}

void Stopwatch::start()
{
    m_start = Clock::counter();
}

void Stopwatch::stop()
{
    m_end = Clock::counter();
}

void Stopwatch::dump(const std::string& label, std::ostream& os)
{
    float duration = (m_end - m_start) * 1000.0f;
    os << label << ":" << duration << "msec." << std::endl;
}
}