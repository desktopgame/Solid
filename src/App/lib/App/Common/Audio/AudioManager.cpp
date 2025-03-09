#include <App/Common/Audio/AudioManager.hpp>

namespace App::Common::Audio {
std::shared_ptr<AudioManager> AudioManager::s_instance;

std::shared_ptr<AudioManager> AudioManager::getInstance()
{
    if (!s_instance) {
        s_instance = std::shared_ptr<AudioManager>(new AudioManager());
        s_instance->init();
    }
    return s_instance;
}

void AudioManager::destroyInstance()
{
    s_instance = nullptr;
}

AudioManager::~AudioManager()
{
}

std::shared_ptr<AudioClip> AudioManager::loadClip(const std::string& file)
{
    if (m_clipMap.count(file) == 0) {
        auto clip = AudioClip::create();
        clip->loadWav(file);
        m_clipMap.insert_or_assign(file, clip);
    }
    return m_clipMap.at(file);
}

void AudioManager::playBgm(const std::string& file)
{
    m_bgmSource->stop();

    m_bgmSource->setClip(loadClip(file));
    m_bgmSource->play();
}
void AudioManager::stopBgm()
{
    m_bgmSource->stop();
}
void AudioManager::playSE(const std::string& file)
{
    for (int32_t i = 0; i < static_cast<int32_t>(m_seSources.size()); i++) {
        auto seSource = m_seSources.at(i);
        if (!seSource->getClip() || !seSource->isPlaying()) {
            seSource->stop();

            seSource->setClip(loadClip(file));
            seSource->play();
            break;
        }
    }
}
// private
AudioManager::AudioManager()
    : m_bgmSource()
    , m_seSources()
    , m_clipMap()
{
}

void AudioManager::init()
{
    m_bgmSource = AudioSource::create();
    m_bgmSource->setLoop(true);
    m_bgmSource->setVolume(0.5f);

    for (int32_t i = 0; i < 10; i++) {
        auto seSource = AudioSource::create();
        seSource->setLoop(false);
        seSource->setVolume(1.0f);
        m_seSources.emplace_back(seSource);
    }
}
}