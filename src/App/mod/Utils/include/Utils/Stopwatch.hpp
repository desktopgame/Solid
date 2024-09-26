#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace Lib::Utils {
class Stopwatch {
public:
    explicit Stopwatch();
    void start();
    void stop();

    void dump(const std::string& label, std::ostream& os);

private:
    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_end;
};
}