#pragma once
#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>
#include <string>

#include <memory>

namespace Leaper
{
    class Log
    {
    public:
        static void Init();

        static inline std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_coreLogger; }
        static inline std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}

#define LP_CORE_LOG(...) ::Leaper::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LP_CORE_LOG_INFO(...) ::Leaper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LP_CORE_LOG_WARNING(...) ::Leaper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LP_CORE_LOG_ERROR(...) ::Leaper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LP_CORE_LOG_CRITICAL(...) ::Leaper::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define LP_LOG(...) ::Leaper::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LP_LOG_INFO(...) ::Leaper::Log::GetClientLogger()->info(__VA_ARGS__)
#define LP_LOG_WARNING(...) ::Leaper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LP_LOG_ERROR(...) ::Leaper::Log::GetClientLogger()->error(__VA_ARGS__)
#define LP_LOG_CRITICAL(...) ::Leaper::Log::GetClientLogger()->critical(__VA_ARGS__)