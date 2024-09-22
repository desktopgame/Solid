#include <OS/Cursor.hpp>
#include <Windows.h>

namespace Lib::OS {

void Cursor::show() { ShowCursor(true); }

void Cursor::hide() { ShowCursor(false); }

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
}

void Cursor::unlock() { ClipCursor(nullptr); }
}