#pragma once
#include <App/library.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace App::Common::Audio {
class AudioManager {
public:
    static std::shared_ptr<AudioManager> getInstance();
    static void destroyInstance();
    ~AudioManager();

    std::shared_ptr<AudioClip> loadClip(const std::string& file);

    void playBgm(const std::string& file);
    void stopBgm();
    void playSE(const std::string& file);

private:
    AudioManager();
    void init();

    static std::shared_ptr<AudioManager> s_instance;

    std::shared_ptr<AudioSource> m_bgmSource;
    std::vector<std::shared_ptr<AudioSource>> m_seSources;
    std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_clipMap;
};
}