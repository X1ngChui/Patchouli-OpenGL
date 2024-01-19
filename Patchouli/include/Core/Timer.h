#pragma once

namespace Pache
{
	class Timer
	{
	public:
		inline Timer()
		{
			reset(); 
		}

		~Timer() = default;

		inline void reset()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		inline float elapsed() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() * 0.001f * 0.001f;
		}

		inline float elapsedMillis() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() * 0.001f;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};

	class ScopedTimer
	{
	public:
		inline ScopedTimer(Identifier name)
			: name(name)
		{
		}

		inline ~ScopedTimer()
		{
			float time = timer.elapsedMillis();
			Log::coreInfo("TIMER {} - {:.3f}ms", name, time);
		}
	private:
		Identifier name;
		Timer timer;
	};
}