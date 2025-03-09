#include <AL/al.h>
#include <AL/alc.h>
#include <Audio/AudioSystem.hpp>

namespace Lib::Audio {
void* AudioSystem::s_device = nullptr;
void* AudioSystem::s_context = nullptr;

void AudioSystem::initialize()
{
    if (!s_device) {
        s_device = alcOpenDevice(nullptr);
        s_context = alcCreateContext((ALCdevice*)s_device, nullptr);

        alcMakeContextCurrent((ALCcontext*)s_context);
    }
}
void AudioSystem::destroy()
{
    if (s_context) {
        alcDestroyContext((ALCcontext*)s_context);
        s_context = nullptr;
    }
    if (s_device) {
        alcCloseDevice((ALCdevice*)s_device);
        s_device = nullptr;
    }
}
}