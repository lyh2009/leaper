#include "console.h"
#include "core/log.h"
#include "imgui.h"
#include <IconsFontAwesome6.h>
#include <function/imgui/ui.h>
#include <string>

const size_t max_shown_ = 1024;
const ImVec4 LVL_LOG_CLR{ 1.0f, 1.0f, 1.0f, 1.0f };

const ImVec4 LVL_DBG_CLR{ 0.0f, 1.0f, 0.8f, 1.0f };
const ImVec4 LVL_INFO_CLR{ 0.0f, 0.8f, 0.0f, 1.0f };
const ImVec4 LVL_WRN_CLR{ 1.0f, 0.7f, 0.0f, 1.0f };
const ImVec4 LVL_ERR_CLR{ 1.0f, 0.0f, 0.0f, 1.0f };

namespace Leaper
{

    void Console::OnAttach() {}

    void DrawMessageTable(const std::string& label, const std::string& time_str, const std::string& str, ImVec4 color, bool is_draw, float table_row)
    {
        if (is_draw)
        {
            ImGui::TableNextRow(0, table_row);
            ImGui::TableSetColumnIndex(0);
            UI::Separator(ImVec2(4.0f, table_row), color);
            ImGui::SameLine();
            ImGui::Text(label.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text(time_str.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text(str.c_str());
        }
    }

    static bool checkbox[4] = { true, true, true, true };
    void Console::OnImGuiRender()
    {
        float scrollY = ImGui::GetScrollY();
        if (scrollY < m_previous_scroll_y)
            m_enable_scroll_to_latest = false;

        if (scrollY >= ImGui::GetScrollMaxY())
            m_enable_scroll_to_latest = true;

        m_previous_scroll_y = scrollY;

        auto messages_ = Leaper::Log::GetSink()->log_item;
        ImGui::Begin(ICON_FA_TERMINAL " Console", nullptr);
        if (ImGui::Button(ICON_FA_TRASH_CAN "Clear"))
        {
            Leaper::Log::GetSink()->log_item.clear();
        }

        auto CheckBox = [](const std::string& lable, bool* b, ImVec4 color = ImVec4(1, 1, 1, 1)) {
            std::string tag = "##" + lable;
            ImGui::Checkbox(tag.c_str(), b);
            ImGui::SameLine();
            ImGui::TextColored(color, lable.c_str());
        };

        ImGui::SameLine();
        CheckBox("Debug", &checkbox[0]);
        ImGui::SameLine();
        CheckBox("Info", &checkbox[1]);
        ImGui::SameLine();
        CheckBox("Warning", &checkbox[2]);
        ImGui::SameLine();
        CheckBox("Error", &checkbox[3]);

        ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        ImGui::BeginChild("ConsoleChlid");

        if (ImGui::BeginTable("table1", 3, flags))
        {
            for (size_t i = messages_.size() > max_shown_ ? messages_.size() - max_shown_ : 0; i < messages_.size(); ++i)
            {
                auto const& msg       = messages_[i];
                const float table_row = 22.0f;
                switch (msg.level)
                {
                case spdlog::level::debug: DrawMessageTable("Debug", msg.time_str, msg.info, LVL_DBG_CLR, checkbox[0], table_row); break;
                case spdlog::level::info: DrawMessageTable("Info", msg.time_str, msg.info, LVL_INFO_CLR, checkbox[1], table_row); break;
                case spdlog::level::warn: DrawMessageTable("Warning", msg.time_str, msg.info, LVL_WRN_CLR, checkbox[2], table_row); break;
                case spdlog::level::err: DrawMessageTable("Error", msg.time_str, msg.info, LVL_ERR_CLR, checkbox[3], table_row); break;
                default: break;
                }
            }

            ImGui::EndTable();
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();
        ImGui::End();
    }
}  // namespace Leaper