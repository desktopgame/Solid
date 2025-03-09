#include <AL/al.h>
#include <Audio/AudioSource.hpp>

namespace Lib::Audio {
std::shared_ptr<AudioSource> AudioSource::create()
{
    auto ret = std::shared_ptr<AudioSource>(new AudioSource());
    ret->init();
    return ret;
}
AudioSource::~AudioSource()
{
    alDeleteSources(1, &m_id);
}
// private
AudioSource::AudioSource()
    : m_id()
{
}

void AudioSource::init()
{
    alGenSources(1, &m_id);
}
}