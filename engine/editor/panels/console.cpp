#include "console.h"
#include "core/log.h"
#include "imgui.h"

void Console::OnAttach()
{
    LP_LOG("LP_LOG");
    LP_LOG_INFO("LP_LOG_INFO");
    LP_LOG_WARNING("LP_LOG_WARNING");
    LP_LOG_ERROR("LP_LOG_ERROR");
}

void Separator(ImVec2 size, ImVec4 color)
{
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::Button("", size);
    ImGui::PopStyleColor();
}

void Console::OnUpdate()
{
    ImGui::Begin(">_Console", nullptr);

    auto messages_          = Leaper::Log::GetSink()->log_item;
    const size_t max_shown_ = 1024;
    const ImVec4 LVL_LOG_CLR{ 1.0f, 1.0f, 1.0f, 1.0f };

    const ImVec4 LVL_DBG_CLR{ 0.0f, 1.0f, 0.8f, 1.0f };
    const ImVec4 LVL_INFO_CLR{ 0.0f, 0.8f, 0.0f, 1.0f };
    const ImVec4 LVL_WRN_CLR{ 1.0f, 0.7f, 0.0f, 1.0f };
    const ImVec4 LVL_ERR_CLR{ 1.0f, 0.0f, 0.0f, 1.0f };

    static ImGuiTableFlags flags =
        ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

    ImGui::BeginChild("ConsoleChlid");
    if (ImGui::BeginTable("table1", 3, flags))
    {
        ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (size_t i = messages_.size() > max_shown_ ? messages_.size() - max_shown_ : 0; i < messages_.size(); ++i)
        {
            auto const& msg       = messages_[i];
            const float table_row = 22.0f;
            ImGui::TableNextRow(0, table_row);
            for (int column = 0; column < 3; column++)
            {
                ImGui::TableSetColumnIndex(column);
                if (column == 0)
                {
                    switch (msg.level)
                    {
                    case spdlog::level::debug:
                        Separator(ImVec2(4.0f, table_row), LVL_DBG_CLR);
                        ImGui::SameLine();
                        ImGui::Text("Debug");
                        break;
                    case spdlog::level::info:
                        Separator(ImVec2(4.0f, table_row), LVL_INFO_CLR);
                        ImGui::SameLine();
                        ImGui::Text("Info ");
                        break;
                    case spdlog::level::warn:
                        Separator(ImVec2(4.0f, table_row), LVL_WRN_CLR);
                        ImGui::SameLine();
                        ImGui::Text("Warning");
                        break;
                    case spdlog::level::err:
                        Separator(ImVec2(4.0f, table_row), LVL_ERR_CLR);
                        ImGui::SameLine();
                        ImGui::Text("Error");
                        break;
                    default:
                        Separator(ImVec2(4.0f, table_row), LVL_LOG_CLR);
                        ImGui::SameLine();
                        ImGui::Text("Log");
                    }
                }

                if (column == 1) ImGui::Text(msg.time_str.c_str());
                if (column == 2) ImGui::Text(msg.info.c_str());
            }
        }

        ImGui::EndTable();
    }
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();
    ImGui::End();
}