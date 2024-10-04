#pragma once
#include <format>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

namespace Leaper
{
    class UI
    {
    public:
        static void Separator(ImVec2 size, ImVec4 color);
        static void BeginColumns();
        static void NextColumns();
        static void EndColumns();
        static bool Button(const std::string& label, const std::string& b_label = "##button", ImVec2 size = { 0.0f, 0.0f });
        static void DragFloat(const std::string& label, float* v, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
        static void DragFloat2(const std::string& label, float* v, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const std::string& format = "%.3f");
        static bool Checkbox(const std::string& label, bool* b);
        static void ColorEdit4(const std::string& label, float* v);
        static void ColorEdit3(const std::string& label, float* v);
        static bool InputText(const std::string& label, std::string v);
        static inline ImRect RectExpanded(const ImRect& rect, float x, float y)
        {
            ImRect result = rect;
            result.Min.x -= x;
            result.Min.y -= y;
            result.Max.x += x;
            result.Max.y += y;
            return result;
        }
        static inline ImRect GetItemRect()
        {
            return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        }
        static void DrawItemActivityOutline(float rounding = 0.0f, bool drawWhenInactive = false, ImColor colourWhenActive = ImColor(80, 80, 80))
        {
            auto* drawList    = ImGui::GetWindowDrawList();
            const ImRect rect = RectExpanded(GetItemRect(), 1.0f, 1.0f);
            if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
            {
                drawList->AddRect(rect.Min, rect.Max, ImColor(60, 60, 60), rounding, 0, 1.5f);
            }
            if (ImGui::IsItemActive())
            {
                drawList->AddRect(rect.Min, rect.Max, colourWhenActive, rounding, 0, 1.0f);
            }
            else if (!ImGui::IsItemHovered() && drawWhenInactive)
            {
                drawList->AddRect(rect.Min, rect.Max, ImColor(50, 50, 50), rounding, 0, 1.0f);
            }
        };

        static void ShiftCursorX(float distance)
        {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
        }

        static void ShiftCursorY(float distance)
        {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
        }

        static void ShiftCursor(float x, float y)
        {
            const ImVec2 cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
        }
        static void BeginDisabled(bool disabled = true)
        {
            if (disabled)
                ImGui::BeginDisabled(true);
        }

        static bool IsItemDisabled()
        {
            return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
        }

        static void EndDisabled()
        {

            if (GImGui->DisabledStackSize > 0)
                ImGui::EndDisabled();
        }

        static void Property(const std::string& label, const std::string& value)
        {
            ShiftCursor(10.0f, 9.0f);
            ImGui::Text(label.c_str());
            ImGui::NextColumn();
            ShiftCursorY(4.0f);
            ImGui::PushItemWidth(-1);
            ImGui::InputText(std::string("##" + label).c_str(), (char*)value.c_str(), value.size(), ImGuiInputTextFlags_ReadOnly);
            if (!IsItemDisabled())
                DrawItemActivityOutline(2.0f, true);
            ImGui::PopItemWidth();
            ImGui::NextColumn();
        }
    };
}  // namespace Leaper