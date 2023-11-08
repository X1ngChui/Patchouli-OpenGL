#include "Core/Layer.h"

namespace Pache
{
	Layer::Layer(const std::string& name)
		: name(name)
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers)
		{
			layer->onDetach();
			delete layer;
		}
	}
}