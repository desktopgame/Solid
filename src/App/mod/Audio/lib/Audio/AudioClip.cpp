#include <AL/al.h>
#include <Audio/AudioClip.hpp>

namespace Lib::Audio {
std::shared_ptr<AudioClip> AudioClip::create()
{
    auto ret = std::shared_ptr<AudioClip>(new AudioClip());
    ret->init();
    return ret;
}
AudioClip::~AudioClip()
{
    alDeleteBuffers(1, &m_id);
}
// private
AudioClip::AudioClip()
    : m_id()
{
}

void AudioClip::init()
{
    alGenBuffers(1, &m_id);
}
}