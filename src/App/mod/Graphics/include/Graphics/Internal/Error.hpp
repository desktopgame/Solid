#pragma once
#include <numeric>

#ifdef _DEBUG
#define STATS(hr) (Lib::Graphics::Internal::Error::show(hr, __FILE__, __LINE__))
#else
#define STATS(hr) (hr)
#endif

namespace Lib::Graphics::Internal {
class Error {
public:
    static int64_t show(int64_t hr, const char* file, int line);

private:
    Error() = delete;
    ~Error() = delete;
};
}