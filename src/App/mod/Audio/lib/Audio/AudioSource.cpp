#include <AL/al.h>
#include <Audio/AudioClip.hpp>
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

void AudioSource::play()
{
    if (m_clip) {
        alSourcePlay(m_clip->getBufferID());
    }
}

void AudioSource::setClip(const std::shared_ptr<AudioClip>& clip) { m_clip = clip; }
std::shared_ptr<AudioClip> AudioSource::getClip() const { return m_clip; }

void AudioSource::setVolume(float volume)
{
    alSourcef(m_id, AL_GAIN, volume);
}
float AudioSource::getVolume() const
{
    ALfloat volume;
    alGetSourcef(m_id, AL_GAIN, &volume);
    return volume;
}

void AudioSource::setLoop(bool isLoop)
{
    alSourcei(m_id, AL_LOOPING, isLoop ? 1 : 0);
}
bool AudioSource::isLoop() const
{
    ALint isLoop;
    alGetSourcei(m_id, AL_LOOPING, &isLoop);
    return isLoop == 1;
}

bool AudioSource::isPlaying() const
{
    ALint state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool AudioSource::isPausing() const
{
    ALint state;
    alGetSourcei(m_id, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
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