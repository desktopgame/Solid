#pragma once
// chronoヘッダーは使えないので、代わりの時計クラス
// c++20からchronoはformatに依存しているのだが、
// chronoとopenmpは同居できないらしい
// しかしdirectxtexを使う都合openmpは必要
// という事情がありchronoは使えない…
#include <Windows.h>
#include <cstdint>

namespace Lib::Utils {
class Clock {
public:
    static float counter();

#if SOLID_ENABLE_INTERNAL
    static void initialize();
    static void destroy();
#endif

private:
    static LARGE_INTEGER s_freq;
    static LARGE_INTEGER s_counter;
    static int64_t s_start;
    static double s_pcFreq;
    Clock();
};
}