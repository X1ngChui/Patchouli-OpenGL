#include "Core/Layer.h"

namespace Pache
{
	Layer::Layer(Identifier name)
		: name(name)
	{
	}

	Layer* LayerStack::Iterator::operator*() const
	{
		return (layerIterator != layers.end()) ? *layerIterator : *overlayIterator;
	}

	LayerStack::Iterator& LayerStack::Iterator::operator++()
	{
		if (layerIterator != layers.end())
			++layerIterator;
		else
			++overlayIterator;
		return *this;
	}

	LayerStack::Iterator LayerStack::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	LayerStack::Iterator& LayerStack::Iterator::operator--()
	{
		if (overlayIterator != overlays.begin())
			--overlayIterator;
		else
			--layerIterator;
		return *this;
	}

	LayerStack::Iterator LayerStack::Iterator::operator--(int)
	{
		Iterator temp = *this;
		--(*this);
		return temp;
	}

	bool LayerStack::Iterator::operator!=(const Iterator& other) const
	{
		return (layerIterator != other.layerIterator) || (overlayIterator != other.overlayIterator);
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers)
		{
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		layers.emplace_back(layer);
		layer->onAttach();
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::ranges::find(layers, layer);
		if (it != layers.end())
		{
			layer->onDetach();
			layers.erase(it);
		}
	}

	void LayerStack::pushOverlay(Overlay* overlay)
	{
		overlays.emplace_back(overlay);
		overlay->onAttach();
	}

	void LayerStack::popOverlay(Overlay* overlay)
	{
		auto it = std::ranges::find(overlays, overlay);
		if (it != overlays.begin())
		{
			overlay->onDetach();
			overlays.erase(it);
		}
	}
}