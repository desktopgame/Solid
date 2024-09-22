#pragma once
#include <OS/Window.hpp>
#include <memory>

namespace Lib::OS {
class Cursor {
public:
    static void show();

    static void hide();

    static bool isVisible();

    static void lock(const std::shared_ptr<Window>& window);

    static void unlock();

    static bool isLocked();

private:
    static bool s_isVisible;
    static bool s_isLocked;

    Cursor() = delete;
    ~Cursor() = delete;
};
}