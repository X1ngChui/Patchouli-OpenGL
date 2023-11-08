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

		const std::string& getName() { return name; }
	protected:
		std::string name;
	};

	template <typename T>
	concept IsLayer = std::derived_from<T, Layer>;

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		template <IsLayer T, typename...Args>
		T* createLayer(Args&&...args)
		{
			Layer* layer = new T(std::forward<Args>(args)...);
			layers.emplace(layers.begin() + (nLayers++), layer);
			layer->onAttach();
			return static_cast<T*>(layer);
		}

		template <IsLayer T>
		void deleteLayer(T* layer)
		{
			auto it = std::find(layers.begin(), layers.begin() + nLayers, layer);
			if (it != layers.end())
			{
				layer->onDetach();
				delete layer;
				layers.erase(it);
				--nLayers;
			}
		}

		template <IsLayer T, typename...Args>
		T* createOverlay(Args&&...args)
		{
			Layer* overlay = new T(std::forward<Args>(args)...);
			layers.emplace_back(overlay);
			return static_cast<T*>(overlay);
		}

		template <IsLayer T>
		void deleteOverlay(T* overlay)
		{
			auto it = std::find(layers.begin() + nLayers, layers.end(), overlay);
			if (it != layers.end())
			{
				overlay->onDetach();
				delete overlay;
				layers.erase(it);
			}
		}
		auto begin() { return layers.begin(); }
		auto end() { return layers.end(); }
	private:
		std::vector<Layer*> layers;
		size_t nLayers = 0;
	};
}