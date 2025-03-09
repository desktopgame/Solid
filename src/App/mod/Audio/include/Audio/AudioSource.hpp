#pragma once
#include <memory>

namespace Lib::Audio {
class AudioClip;
class AudioSource {
public:
    static std::shared_ptr<AudioSource> create();
    ~AudioSource();

    void play();

    void setClip(const std::shared_ptr<AudioClip>& clip);
    std::shared_ptr<AudioClip> getClip() const;

private:
    AudioSource();
    void init();

    uint32_t m_id;
    std::shared_ptr<AudioClip> m_clip;
};
}