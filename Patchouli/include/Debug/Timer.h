#pragma once

namespace Pache
{
	class Timer
	{
	public:
		inline Timer(const char* name)
			: name(name)
		{
		}

		~Timer() = default;

		inline void start()
		{
			startTime = std::chrono::high_resolution_clock::now();
		}

		inline void end()
		{
			endTime = std::chrono::high_resolution_clock::now();
		}

		inline float getDuration() const 
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		}
	private:
		const char* name;
		std::chrono::time_point<std::chrono::steady_clock> startTime;
		std::chrono::time_point<std::chrono::steady_clock> endTime;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name)
			: name(name)
		{
			startTime = std::chrono::high_resolution_clock::now();
		}

		~ScopedTimer()
		{
			endTime = std::chrono::high_resolution_clock::now();
		}
	private:
		const char* name;
		std::chrono::time_point<std::chrono::steady_clock> startTime;
		std::chrono::time_point<std::chrono::steady_clock> endTime;
	};
}