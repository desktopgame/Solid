#pragma once
#include <OS/Window.hpp>
#include <memory>

namespace Lib::OS {
namespace Cursor {
    void show();

    void hide();

    void lock(const std::shared_ptr<Window>& window);

    void unlock();
}
}