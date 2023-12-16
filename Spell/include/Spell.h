#include "Patchouli.h"

namespace Spell
{
	class ExampleLayer : public Pache::Layer
	{
	public:
		ExampleLayer();

		virtual void onUpdate(Pache::Timestep timestep) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Pache::Event& evt) override;
	private:
		Pache::Ref<Pache::VertexBuffer> vertexBuffer;
		Pache::Ref<Pache::IndexBuffer> indexBuffer;
		Pache::Ref<Pache::VertexArray> vertexArray;

		Pache::ShaderLibruary shaderLibruary;
		Pache::Ref<Pache::Texture2D> texture;

		Pache::OrthographicCameraController cameraController;
	};

	class Spell : public Pache::Application
	{
	public:
		Spell()
		{
			pushLayer(new ExampleLayer);
		}

		~Spell() = default;
	};
}

Pache::Application* Pache::createApplication()
{
	return new Spell::Spell;
}