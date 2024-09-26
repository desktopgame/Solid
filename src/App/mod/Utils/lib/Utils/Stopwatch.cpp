#include <Utils/Stopwatch.hpp>

namespace Lib::Utils {
// public
Stopwatch::Stopwatch()
{
}

void Stopwatch::start()
{
    m_start = std::chrono::system_clock::now();
}

void Stopwatch::stop()
{
    m_end = std::chrono::system_clock::now();
}

void Stopwatch::dump(const std::string& label, std::ostream& os)
{
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);
    os << label << ":" << duration.count() << "msec." << std::endl;
}
}