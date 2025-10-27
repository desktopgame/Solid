#pragma once
#include <App/library.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace App::Common::Audio {
/**
 * オーディオの再生と停止を管理するクラスです。
 */
class AudioManager {
public:
    /**
     * 唯一のインスタンスを返します。
     * @return
     */
    static std::shared_ptr<AudioManager> getInstance();

    /**
     * 唯一のインスタンスを破棄します。
     */
    static void destroyInstance();
    ~AudioManager();

    /**
     * 指定のファイルを.wavフォーマットとして読み込み、辞書へ登録します。
     * @param file
     */
    std::shared_ptr<AudioClip> loadClip(const std::string& file);

    /**
     * 既に再生中のものがあれば停止してから指定のBGMを再生します。
     * @param file
     */
    void playBgm(const std::string& file);

    /**
     * BGMを停止します。
     */
    void stopBgm();

    /**
     * 指定のSEを再生します。
     * @param file
     */
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