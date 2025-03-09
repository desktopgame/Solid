#pragma once
#include <memory>

namespace Lib::Audio {
class AudioClip;
class AudioSource {
public:
    static std::shared_ptr<AudioSource> create();
    ~AudioSource();

    void play();
    void pause();
    void resume();

    void setClip(const std::shared_ptr<AudioClip>& clip);
    std::shared_ptr<AudioClip> getClip() const;

    void setVolume(float volume);
    float getVolume() const;

    void setLoop(bool isLoop);
    bool isLoop() const;

    bool isPlaying() const;
    bool isPausing() const;

private:
    AudioSource();
    void init();

    uint32_t m_id;
    std::shared_ptr<AudioClip> m_clip;
};
}