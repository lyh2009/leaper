#include "imgui.h"
#include "ui.h"

namespace Leaper
{
    static float column_width = 85.0f;

    void UI::Separator(ImVec2 size, ImVec4 color)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::Button("", size);
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
    }
    void UI::BeginColumns()
    {

        ImGui::Columns(2);
    }
    void UI::NextColumns()
    {
        ImGui::NextColumn();
    }
    void UI::EndColumns()
    {
        ImGui::Columns(1);
    }

    bool UI::Checkbox(const std::string& label, bool* b)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        if (ImGui::Checkbox(std::string("##" + label).c_str(), b))
            return true;
        if (!IsItemDisabled())
            DrawItemActivityOutline(2.0f, true);
        NextColumns();
        return false;
    }
    void UI::ColorEdit4(const std::string& label, float* v)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        ImGui::ColorEdit4(std::string("##" + label).c_str(), v);
        NextColumns();
    }
    void UI::ColorEdit3(const std::string& label, float* v)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        ImGui::ColorEdit3(std::string("##" + label).c_str(), v);
        NextColumns();
    }
    bool UI::InputText(const std::string& label, std::string v)
    {
        bool returnValue;
        return returnValue;
    }

    bool UI::Button(const std::string& label, const std::string& b_label, ImVec2 size)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        if (ImGui::Button(b_label.c_str(), size))
            return true;
        if (!IsItemDisabled())
            DrawItemActivityOutline(2.0f, true);
        NextColumns();
        return false;
    }
    void UI::DragFloat(const std::string& label, float* v, float speed, float min, float max, const std::string& format)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        ImGui::DragFloat(std::string("##" + label).c_str(), v, speed, min, max, format.c_str());
        if (!IsItemDisabled())
            DrawItemActivityOutline(2.0f, true);
        NextColumns();
    }
    void UI::DragFloat2(const std::string& label, float* v, float speed, float min, float max, const std::string& format)
    {
        ImGui::Text(label.c_str());
        NextColumns();
        ImGui::DragFloat2(std::string("##" + label).c_str(), v, speed, min, max, format.c_str());
        if (!IsItemDisabled())
            DrawItemActivityOutline(2.0f, true);
        NextColumns();
    }
}  // namespace Leaper