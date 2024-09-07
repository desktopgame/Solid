#include <Graphics/Gui.hpp>
#include <imgui.h>

namespace Lib::Graphics::Gui {
void showDemoWindow(bool& isGuiOpen)
{
    ImGui::ShowDemoWindow(&isGuiOpen);
}
}