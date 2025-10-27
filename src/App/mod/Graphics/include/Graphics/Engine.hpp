#pragma once
#include <memory>
#include <mutex>
#include <thread>

namespace Lib::OS {
class Window;
}

namespace Lib::Graphics {
class Device;
/**
 * Solidエンジンのエントリポイントとなるクラスです。
 */
class Engine {
public:
    ~Engine();
    /**
     * エンジンのインスタンスを返します。
     * @return
     */
    static std::shared_ptr<Engine> getInstance();

    /**
     * エンジンが存在しなければ例外を投げます。
     */
    static void require();

    /**
     * エンジンを開始します。
     * @param argc
     * @param argv
     * @return
     */
    std::shared_ptr<Engine> startup(int argc, char* argv[]);

    /**
     * エンジンを終了します。
     */
    void shutdown();

    /**
     * ウィンドウを返します。
     * @return
     */
    std::shared_ptr<OS::Window> getWindow() const;

    /**
     * デバイスを返します。
     * @return
     */
    std::shared_ptr<Device> getDevice() const;

private:
    static std::mutex s_mutex;
    static std::shared_ptr<Engine> s_instance;
    Engine();

    bool m_started;
    bool m_shutdowned;
    std::shared_ptr<OS::Window> m_window;
    std::shared_ptr<Device> m_device;
};
}