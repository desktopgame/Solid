#include <AL/al.h>
#include <Audio/AudioClip.hpp>
#include <fstream>
#include <stdexcept>

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
    releaseData();
}

void AudioClip::loadWav(const std::string& file)
{
    releaseData();
    // see: https://stackoverflow.com/questions/36949957/loading-a-wav-file-for-openal
    char buffer[4];
    std::ifstream in(file, std::ios::binary);
    in.read(buffer, 4);
    if (strncmp(buffer, "RIFF", 4) != 0) {
        throw std::invalid_argument("illegal format.");
    }
    in.read(buffer, 4);
    in.read(buffer, 4); // WAVE
    in.read(buffer, 4); // fmt
    in.read(buffer, 4); // 16
    in.read(buffer, 2); // 1
    in.read(buffer, 2);
    int32_t chan = convertToInt(buffer, 2);
    in.read(buffer, 4);
    int32_t samplerate = convertToInt(buffer, 4);
    in.read(buffer, 4);
    in.read(buffer, 2);
    in.read(buffer, 2);
    int32_t bps = convertToInt(buffer, 2);
    in.read(buffer, 4); // data
    in.read(buffer, 4);
    int32_t size = convertToInt(buffer, 4);
    m_data = new char[size];
    in.read(m_data, size);

    ALenum format;
    if (chan == 1) {
        if (bps == 8) {
            format = AL_FORMAT_MONO8;
        } else {
            format = AL_FORMAT_MONO16;
        }
    } else {
        if (bps == 8) {
            format = AL_FORMAT_STEREO8;
        } else {
            format = AL_FORMAT_STEREO16;
        }
    }
    alBufferData(m_id, format, m_data, size, samplerate);
}

uint32_t AudioClip::getBufferID() const
{
    return m_id;
}

// private
AudioClip::AudioClip()
    : m_id()
    , m_data(nullptr)
{
}

void AudioClip::init()
{
    alGenBuffers(1, &m_id);
}

void AudioClip::releaseData()
{
    if (m_data) {
        delete m_data;
        m_data = nullptr;
    }
}

bool AudioClip::isBigEndian()
{
    // see: https://stackoverflow.com/questions/36949957/loading-a-wav-file-for-openal
    int32_t a = 1;
    return !((char*)&a)[0];
}

int32_t AudioClip::convertToInt(char* buffer, int32_t len)
{
    // see: https://stackoverflow.com/questions/36949957/loading-a-wav-file-for-openal
    int32_t a = 0;
    if (!isBigEndian())
        for (int32_t i = 0; i < len; i++)
            ((char*)&a)[i] = buffer[i];
    else
        for (int32_t i = 0; i < len; i++)
            ((char*)&a)[3 - i] = buffer[i];
    return a;
}
}