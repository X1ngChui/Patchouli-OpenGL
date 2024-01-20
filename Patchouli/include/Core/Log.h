#pragma once
#include "spdlog/spdlog.h"

#ifdef _MSC_VER
	#define trap() __debugbreak()
#endif
#ifdef __GNUC__
	#define trap() __builtin_trap();
#endif

namespace Pache
{
	// A utility class for logging messages using the spdlog library.
	// Logging utility class with two categories: Core (engine internals) and Client (user application).
	// Provides logging at five levels: Trace, Info, Warn, Error, and Critical.
	// Assertion functions are also available.
	// Console output is only enabled in Debug mode.

	// Example of using logging functions:
	// Log::clientError	("The world is full of secrets.");			(Output: The world is full of secrets.)
	// Log::coreInfo	("{0} = {1}.", "Cirno", 9);					(Output: Cirno = 9.)
	// Log::clientAssert(false, "{}!!!", "Magicannon");				(Output: Magicannon!!!)
	class Log
	{
	public:
		static void init();

		template <typename... Args>
		static void coreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			coreLogger->trace(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void coreTrace(T&& msg)
		{
			coreLogger->trace(std::forward<T>(msg));
		}

		template <typename... Args>
		static void coreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			coreLogger->info(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void coreInfo(T&& msg)
		{
			coreLogger->info(std::forward<T>(msg));
		}

		template <typename... Args>
		static void coreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			coreLogger->warn(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void coreWarn(T&& msg)
		{
			coreLogger->warn(std::forward<T>(msg));
		}

		template <typename... Args>
		static void coreError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			coreLogger->error(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void coreError(T&& msg)
		{
			coreLogger->error(std::forward<T>(msg));
		}

		template <typename... Args>
		static void coreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			coreLogger->critical(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void coreCritical(T&& msg)
		{
			coreLogger->critical(std::forward<T>(msg));
		}

		template <typename... Args>
		static void coreAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			if (!assertion)
			{
				coreLogger->error(fmt, std::forward<Args>(args)...);
				trap();
			}
		}

		template <typename T>
		static void coreAssert(bool assertion, T&& msg)
		{
			if (!assertion)
			{
				coreLogger->error(std::forward<T>(msg));
				trap();
			}
		}

		template <typename... Args>
		static void clientTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			clientLogger->trace(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void clientTrace(T&& msg)
		{
			clientLogger->trace(std::forward<T>(msg));
		}

		template <typename... Args>
		static void clientInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			clientLogger->info(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void clientInfo(T&& msg)
		{
			clientLogger->info(std::forward<T>(msg));
		}

		template <typename... Args>
		static void clientWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			clientLogger->warn(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void clientWarn(T&& msg)
		{
			clientLogger->warn(std::forward<T>(msg));
		}

		template <typename... Args>
		static void clientError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			clientLogger->error(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void clientError(T&& msg)
		{
			clientLogger->error(std::forward<T>(msg));
		}

		template <typename... Args>
		static void clientCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			clientLogger->critical(fmt, std::forward<Args>(args)...);
		}

		template <typename T>
		static void clientCritical(T&& msg)
		{
			clientLogger->critical(std::forward<T>(msg));
		}

		template <typename... Args>
		static void clientAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			if (!assertion)
			{
				clientLogger->error(fmt, std::forward<Args>(args)...);
				trap();
			}
		}

		template <typename T>
		static void clientAssert(bool assertion, T&& msg)
		{
			if (!assertion)
			{
				clientLogger->error(std::forward<T>(msg));
				trap();
			}
		}

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}