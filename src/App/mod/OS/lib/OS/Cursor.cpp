#include <OS/Cursor.hpp>
#include <Windows.h>

namespace Lib::OS {
bool Cursor::s_isVisible = true;
bool Cursor::s_isLocked = false;

void Cursor::show()
{
    ShowCursor(true);
    s_isVisible = true;
}

void Cursor::hide()
{
    ShowCursor(false);
    s_isVisible = false;
}

bool Cursor::isVisible() { return s_isVisible; }

void Cursor::lock(const std::shared_ptr<Window>& window)
{
    RECT rect;
    GetClientRect(window->getHWND(), &rect);

    POINT ul;
    ul.x = rect.left;
    ul.y = rect.top;

    POINT lr;
    lr.x = rect.right;
    lr.y = rect.bottom;

    MapWindowPoints(window->getHWND(), nullptr, &ul, 1);
    MapWindowPoints(window->getHWND(), nullptr, &lr, 1);

    rect.left = ul.x;
    rect.top = ul.y;

    rect.right = lr.x;
    rect.bottom = lr.y;

    ClipCursor(&rect);
    s_isLocked = true;
}

void Cursor::unlock()
{
    ClipCursor(nullptr);
    s_isLocked = false;
}

bool Cursor::isLocked() { return s_isLocked; }
}