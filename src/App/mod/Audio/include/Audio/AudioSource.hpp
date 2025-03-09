#pragma once
#include <memory>

namespace Lib::Audio {
class AudioSource {
public:
    static std::shared_ptr<AudioSource> create();
    ~AudioSource();

private:
    AudioSource();
    void init();

    uint32_t m_id;
};
}