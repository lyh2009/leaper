#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Leaper::Log::s_coreLogger;
std::shared_ptr<spdlog::logger> Leaper::Log::s_clientLogger;

void Leaper::Log::Init()
{
	spdlog::set_pattern("%^[%Y/%m/%d/%T][%n][%l]: %v%$");
	s_coreLogger = spdlog::stdout_color_mt("leaper");
	s_coreLogger->set_level(spdlog::level::trace);
 
	s_clientLogger = spdlog::stdout_color_mt("console");
	s_clientLogger->set_level(spdlog::level::trace);
}