#include "log.h"

#include <string>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<spdlog::logger> Leaper::Log::s_coreLogger;
std::shared_ptr<spdlog::logger> Leaper::Log::s_clientLogger;
std::vector<spdlog::sink_ptr> Leaper::Log::s_log_sinks;

void Leaper::Log::Init()
{

	s_log_sinks.emplace_back(std::make_shared<Leaper::Sink_mt>());
	s_log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	s_log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("leaper.log", true));

	s_log_sinks[0]->set_pattern("%^[%Y/%m/%d/%T][%n][%l]: %v%$");
	s_log_sinks[1]->set_pattern("%^[%Y/%m/%d/%T][%n][%l]: %v%$");
	s_log_sinks[2]->set_pattern("%^[%Y/%m/%d/%T][%n][%l]: %v%$");

	s_coreLogger = std::make_shared<spdlog::logger>("leaper", std::begin(s_log_sinks), std::end(s_log_sinks));
	spdlog::register_logger(s_coreLogger);
	s_coreLogger->set_level(spdlog::level::trace);
	s_coreLogger->flush_on(spdlog::level::trace);

	s_clientLogger = std::make_shared<spdlog::logger>("app", std::begin(s_log_sinks), std::end(s_log_sinks));
	spdlog::register_logger(s_clientLogger);
	s_clientLogger->set_level(spdlog::level::trace);
	s_clientLogger->flush_on(spdlog::level::trace);
}