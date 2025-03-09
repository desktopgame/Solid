#include <AL/al.h>
#include <Audio/AudioClip.hpp>
#include <fstream>
#include <stdexcept>

// RIFFヘッダー（最初の12バイト）: "RIFF", ファイルサイズ, フォーマット識別子
struct RIFFHeader {
    char chunkId[4]; // "RIFF"
    uint32_t chunkSize; // ファイルサイズ - 8
    char formType[4]; // 通常は "WAVE"
};

// 各チャンクのヘッダー（8バイト）: チャンクIDとチャンクサイズ
struct ChunkHeader {
    char id[4];
    uint32_t size;
};

// fmtチャンクの基本フォーマット（PCMの場合、16バイト）
struct FmtChunk {
    uint16_t audioFormat; // PCMなら1
    uint16_t numChannels; // チャンネル数
    uint32_t sampleRate; // サンプルレート
    uint32_t byteRate; // サンプルレート * チャンネル数 * bitsPerSample/8
    uint16_t blockAlign; // チャンネル数 * bitsPerSample/8
    uint16_t bitsPerSample; // 1サンプルあたりのビット数
};

// 4バイトのIDを比較するヘルパー関数
static bool compareId(const char* id, const char* cmp)
{
    return std::strncmp(id, cmp, 4) == 0;
}

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
    std::ifstream in(file, std::ios::binary);

    // RIFFヘッダーの読み込み
    RIFFHeader riff;
    in.read(reinterpret_cast<char*>(&riff), sizeof(riff));
    if (!in) {
        throw std::invalid_argument("illegal format");
    }

    // "RIFF"かどうかチェック
    if (!compareId(riff.chunkId, "RIFF")) {
        throw std::invalid_argument("illegal format");
    }

    // フォーマット識別子のチェック
    if (!compareId(riff.formType, "WAVE")) {
        // LogicProの場合、"WAVE"の代わりに"JUNK"が入る可能性がある
        if (compareId(riff.formType, "JUNK")) {
            // 次のチャンクヘッダーを読み込む
            ChunkHeader nextHeader;
            in.read(reinterpret_cast<char*>(&nextHeader), sizeof(nextHeader));
            if (!in) {
                throw std::invalid_argument("illegal format");
            }
            // もし次のチャンクが実際のJUNKチャンクなら、そのサイズ分をスキップ
            if (compareId(nextHeader.id, "JUNK")) {
                in.seekg(nextHeader.size, std::ios::cur);
            } else {
                // JUNKチャンクでなければ、シーク位置を戻して再解釈
                in.seekg(-static_cast<int>(sizeof(ChunkHeader)), std::ios::cur);
            }
        } else {
            throw std::invalid_argument("illegal format");
        }
    }

    // 取得したい情報
    uint16_t numChannels = 0;
    uint16_t bitsPerSample = 0;
    uint32_t sampleRate = 0;
    uint32_t dataSize = 0;
    void* audioData = nullptr;

    // ファイル内の各チャンクを順次処理
    while (in) {
        ChunkHeader header;
        in.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (in.gcount() < sizeof(header))
            break; // 最後に到達

        std::string chunkId(header.id, 4);

        if (compareId(header.id, "JUNK")) {
            // JUNKチャンクの場合は、サイズ分をスキップする
            in.seekg(header.size, std::ios::cur);
        } else if (compareId(header.id, "fmt ")) {
            // fmtチャンクの処理
            if (header.size < sizeof(FmtChunk)) {
                in.seekg(header.size, std::ios::cur);
            } else {
                FmtChunk fmt;
                in.read(reinterpret_cast<char*>(&fmt), sizeof(FmtChunk));
                // header.sizeが16バイトより大きい場合、拡張データはスキップ
                if (header.size > sizeof(FmtChunk)) {
                    in.seekg(header.size - sizeof(FmtChunk), std::ios::cur);
                }
                numChannels = fmt.numChannels;
                sampleRate = fmt.sampleRate;
                bitsPerSample = fmt.bitsPerSample;
            }
        } else if (compareId(header.id, "data")) {
            // dataチャンクの処理（オーディオデータの読み込み）
            dataSize = header.size;
            // dataチャンクのサイズ分のメモリを動的に確保（void*として取得）
            audioData = new char[dataSize];
            in.read(reinterpret_cast<char*>(audioData), dataSize);
        } else {
            // その他のチャンクは不明な場合、サイズ分をスキップ
            in.seekg(header.size, std::ios::cur);
        }
    }
    in.close();

    ALenum format;
    if (numChannels == 1) {
        if (bitsPerSample == 8) {
            format = AL_FORMAT_MONO8;
        } else {
            format = AL_FORMAT_MONO16;
        }
    } else {
        if (bitsPerSample == 8) {
            format = AL_FORMAT_STEREO8;
        } else {
            format = AL_FORMAT_STEREO16;
        }
    }
    alBufferData(m_id, format, audioData, dataSize, sampleRate);
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