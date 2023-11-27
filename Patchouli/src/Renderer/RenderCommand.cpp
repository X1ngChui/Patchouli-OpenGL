#include "Renderer/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Pache
{
	RendererAPI* RenderCommand::api = new OpenGLRendererAPI;
}