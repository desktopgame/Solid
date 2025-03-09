#pragma once
#include <memory>

namespace Lib::Audio {
class AudioClip {
public:
    static std::shared_ptr<AudioClip> create();
    ~AudioClip();

private:
    AudioClip();
    void init();

    uint32_t m_id;
};
}