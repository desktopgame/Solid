#pragma once
#include <memory>
#include <string>

namespace Lib::Audio {
class AudioClip {
public:
    static std::shared_ptr<AudioClip> create();
    ~AudioClip();

    void loadWav(const std::string& file);

private:
    AudioClip();
    void init();
    void releaseData();

    static bool isBigEndian();
    static int32_t convertToInt(char* buffer, int32_t len);

    uint32_t m_id;
    char* m_data;
};
}