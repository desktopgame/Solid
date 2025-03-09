#pragma once
#include <memory>

namespace Lib::Audio {
class AudioSystem {
public:
    static void initialize();
    static void destroy();

private:
    AudioSystem() = delete;
    ~AudioSystem() = delete;

    static void* s_device;
    static void* s_context;
};
}