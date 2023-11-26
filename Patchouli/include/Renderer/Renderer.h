#pragma once

namespace Pache
{
	enum class RendererAPI
	{
		None = 0, OpenGL
	};

	class Renderer
	{
	public:
		static RendererAPI getAPI() { return api; }
	private:
		static RendererAPI api;
	};
}