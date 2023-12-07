#pragma once

namespace Pache
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: time(time)
		{
		}

		float toMilliSeconds() const { return time * 1000.0f; }
		float toSeconds() const { return time; }

		operator float() const { return time; }
	private:
		float time;
	};
}