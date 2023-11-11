#pragma once

namespace Pache
{
	class Layer
	{
	public:
		Layer(const std::string& name = "New Layer");
		virtual ~Layer() = default;

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onRender() {}
		virtual void onEvent(Event& evt) {}

		const std::string& getName() const { return name; }
	protected:
		std::string name;
	};

	using Overlay = Layer;

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

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

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);

		void pushOverlay(Overlay* overlay);
		void popOverlay(Overlay* overlay);

		auto begin() { return Iterator(layers, overlays, layers.begin(), overlays.begin()); }
		auto end() { return Iterator(layers, overlays, layers.end(), overlays.end()); }
	private:
		std::vector<Layer*> layers;
		std::vector<Overlay*> overlays;
	};
}