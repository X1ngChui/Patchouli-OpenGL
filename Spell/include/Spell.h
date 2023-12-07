#include "Patchouli.h"

namespace Spell
{
	class ExampleLayer : public Pache::Layer
	{
	public:
		ExampleLayer();

		virtual void onUpdate(Pache::Timestep timestep) override;
		virtual void onImGuiRender() override;
	private:
		std::shared_ptr<Pache::VertexBuffer> vertexBuffer;
		std::shared_ptr<Pache::IndexBuffer> indexBuffer;
		std::shared_ptr<Pache::VertexArray> vertexArray;
		std::shared_ptr<Pache::Shader> shader;

		Pache::OrthographicCamera camera;

		glm::vec3 color = { 1.0f, 1.0f, 1.0f };
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