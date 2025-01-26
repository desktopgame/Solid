#pragma once
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
    float m_start;
    float m_end;
};
}