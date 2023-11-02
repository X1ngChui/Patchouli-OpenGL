#include "Core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Pache
{
	std::shared_ptr<spdlog::logger> Log::coreLogger;
	std::shared_ptr<spdlog::logger> Log::clientLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] [%l] [%n] %v%$");
		coreLogger = spdlog::stdout_color_mt("Pache");
		clientLogger = spdlog::stdout_color_mt("Client");
	}
}