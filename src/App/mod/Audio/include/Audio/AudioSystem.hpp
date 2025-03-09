#pragma once

namespace Lib::Audio {
class AudioSystem {
public:
    static void initialize();
    static void destroy();

private:
    AudioSystem() = delete;
    ~AudioSystem() = delete;
};
}