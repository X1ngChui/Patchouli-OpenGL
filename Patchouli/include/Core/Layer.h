#pragma once
#include "Identifier.h"
#include "Timestep.h"

namespace Pache
{
	// Layers can be attached to or detached from the applicaiton,
	// responsible for updating, rendering, and handling events.
	class Layer
	{
	public:
		Layer(Identifier name = LITERAL_IDENTIFIER("New Layer"));
		virtual ~Layer() = default;

		// Called when the layer is attached to the layer stack.
		virtual void onAttach() {}
		// Called when the layer is detached from the layer stack.
		virtual void onDetach() {}
		// Called every frame to update the layer.
		virtual void onUpdate(Timestep timestep) {}
		// Called every frame to render the ImGui.
		virtual void onImGuiRender() {}
		// Called to handle events for the layer.
		virtual void onEvent(Event& evt) {}

		Identifier getName() const { return name; }
	protected:
		Identifier name;
	};

	// An overlay is essentially a specialized type of layer,
	// often used for UI and debugging features.
	using Overlay = Layer;

	// LayerStack is respoinsible for managing layers and overlays.
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		// Iterator for layer stack. Forward iteration starts with
		// layers and then overlays, preserving the order of insertionm.
		// Reverse iteration starts with overlays and then layers,
		// in the reverse order of insertion.
		class Iterator
		{
		public:
			Iterator(const std::vector<Layer*>& layers, const std::vector<Overlay*>& overlays, std::vector<Layer*>::const_iterator layerIterator, std::vector<Overlay*>::const_iterator overlayIterator)
				: layers(layers), overlays(overlays), layerIterator(layerIterator), overlayIterator(overlayIterator)
			{
			}

			Layer* operator*() const;
			Iterator& operator++();
			Iterator operator++(int);
			Iterator& operator--();
			Iterator operator--(int);
			bool operator!=(const Iterator& other) const;

		private:
			const std::vector<Layer*>& layers;
			const std::vector<Overlay*>& overlays;
			std::vector<Layer*>::const_iterator layerIterator;
			std::vector<Overlay*>::const_iterator overlayIterator;
		};

		// Add a layer to the layer stack.
		void pushLayer(Layer* layer);
		// Remove a layer from the layer stack.
		void popLayer(Layer* layer);
		// Add a overlay to the layer stack.
		void pushOverlay(Overlay* overlay);
		// Remove a layer from the layer stack.
		void popOverlay(Overlay* overlay);
		
		// Get the beginning iterator for the layer stack.
		auto begin() { return Iterator(layers, overlays, layers.begin(), overlays.begin()); }
		// Get the ending iterator for the layer stack.
		auto end() { return Iterator(layers, overlays, layers.end(), overlays.end()); }
	private:
		std::vector<Layer*> layers;
		std::vector<Overlay*> overlays;
	};
}