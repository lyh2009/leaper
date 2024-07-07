#include "lppch.h"
#ifdef _WIN32
#include <platform/windows/windows_window.h>
#endif

#include "window.h"

Leaper::Ref<Leaper::Window> Leaper::Window::Create(uint32_t width, uint32_t height, std::string title)
{
#ifdef _WIN32
    return Leaper::CreateRef<WindowsWindow>(title, width, height);
#else
    LP_ASSERT(false, "Unknow platform!");
    return nullptr;
#endif
}