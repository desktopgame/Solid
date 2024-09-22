#pragma once
#include <OS/Window.hpp>
#include <memory>

namespace Lib::OS {
class Cursor {
public:
    static void show();

    static void hide();

    static void lock(const std::shared_ptr<Window>& window);

    static void unlock();

private:
    Cursor() = delete;
    ~Cursor() = delete;
};
}