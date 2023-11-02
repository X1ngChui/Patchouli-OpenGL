#pragma once
#include "spdlog/spdlog.h"

namespace Pache
{
	class Log
	{
	public:
		static void init();

		template <typename... Args>
		static void coreTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			coreLogger->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void coreInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			coreLogger->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void coreWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			coreLogger->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void coreError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			coreLogger->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void coreCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			coreLogger->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void coreAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			if (!assertion)
			{
				coreLogger->error("Assertion failed: {0}", fmt::format(fmt, std::forward<Args>(args)...));
#ifdef PACHE_COMPILER_MSVC
				__debugbreak();
#endif
#if defined(PACHE_COMPILER_GNU) || defined(PACHE_COMPILER_CLANG)
				__builtin_trap();
#endif
			}
#endif
		}

		template <typename... Args>
		static void clientTrace(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			clientLogger->trace(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void clientInfo(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			clientLogger->info(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void clientWarn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			clientLogger->warn(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void clientError(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			clientLogger->error(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void clientCritical(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			clientLogger->critical(fmt, std::forward<Args>(args)...);
#endif
		}

		template <typename... Args>
		static void clientAssert(bool assertion, spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#ifdef PACHE_DEBUG
			if (!assertion)
			{
				clientLogger->error("Assertion failed: {0}", fmt::format(fmt, std::forward<Args>(args)...));
#ifdef PACHE_COMPILER_MSVC
				__debugbreak();
#endif
#if defined(PACHE_COMPILER_GNU) || defined(PACHE_COMPILER_CLANG)
				__builtin_trap();
#endif
			}
#endif
		}

	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}