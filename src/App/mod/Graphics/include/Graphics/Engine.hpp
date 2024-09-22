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
 * @brief Engine class.
 */
class Engine {
public:
    ~Engine();
    /**
     * @brief Get the Engine object
     * @return Instance
     */
    static std::shared_ptr<Engine> getInstance();

    /**
     * @brief throw exception if instance not exist.
     */
    static void require();

    /**
     * @brief startup Engine.
     * @param argc
     * @param argv
     */
    std::shared_ptr<Engine> startup(int argc, char* argv[]);

    /**
     * @brief shutdown Engine.
     */
    void shutdown();

    /**
     * @brief Get the Window object
     * @return std::shared_ptr<Window>
     */
    std::shared_ptr<OS::Window> getWindow() const;

    /**
     * @brief Get the Device object
     * @return std::shared_ptr<Window>
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