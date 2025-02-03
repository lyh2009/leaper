#pragma once
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

#include <core/time.h>

#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace Leaper
{
    class Sink_mt : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        struct LogItem
        {
            std::string message;
            std::string time_str;
            std::string info;
            spdlog::level::level_enum level;
        };

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            LogItem it;
            it.time_str = Leaper::Time::GetTime("%Y-%m-%d%H:%M:%S ");
            it.info     = std::string(msg.payload.data(), msg.payload.size());
            it.message  = it.time_str + it.info;
            it.level    = msg.level;
            log_item.push_back(it);
        };

        void flush_() override {};

    public:
        std::vector<LogItem> log_item;
    };

    class Log
    {
    public:
        static void Init();

        [[nodiscard]] static inline std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_coreLogger;
        }
        [[nodiscard]] static inline std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_clientLogger;
        }

        static inline const std::shared_ptr<Sink_mt> GetSink()
        {
            return std::dynamic_pointer_cast<Sink_mt>(s_log_sinks[0]);
        }

    private:
        static std::vector<spdlog::sink_ptr> s_log_sinks;
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}  // namespace Leaper

#define LP_CORE(...) ::Leaper::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LP_CORE_INFO(...) ::Leaper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LP_CORE_WARNING(...) ::Leaper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LP_CORE_ERROR(...) ::Leaper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LP_CORE_CRITICAL(...) ::Leaper::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LP(...) ::Leaper::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LP_INFO(...) ::Leaper::Log::GetClientLogger()->info(__VA_ARGS__)
#define LP_WARNING(...) ::Leaper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LP_ERROR(...) ::Leaper::Log::GetClientLogger()->error(__VA_ARGS__)
#define LP_CRITICAL(...) ::Leaper::Log::GetClientLogger()->critical(__VA_ARGS__)