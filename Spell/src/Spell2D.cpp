#include "Spell2D.h"
#include "imgui.h"

namespace Spell
{
	Spell2D::Spell2D()
		: Layer(LITERAL_IDENTIFIER("Spell 2D")), cameraController(1280.0f / 720.0f)
	{
	}

	void Spell2D::onAttach()
	{
		texture = Pache::Texture2D::create("assets/textures/hikari.png");
	}

	void Spell2D::onDetach()
	{
	}

	void Spell2D::onUpdate(Pache::Timestep ts)
	{
		// static float rotation = 0.0f;
		// rotation += ts * (3.14f / 2.0f);

		Pache::ScopedTimer timer(LITERAL_IDENTIFIER("Spell2D::onUpdate"));

		cameraController.onUpdate(ts);

		Pache::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Pache::RenderCommand::clear();

		Pache::Renderer2D::beginScene(cameraController.getCamera());

		Pache::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, texture, 2.5f, {0.8f, 0.6f, 0.6f, 1.0f});
		// Pache::Renderer2D::drawQuad({ -0.0f, -0.0f, 0.0f }, { 1.0f, 1.0f }, color);
		// Pache::Renderer2D::drawRotatedQuad({ -0.0f, -0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, color);
		// Pache::Renderer2D::drawRotatedQuad({ -0.5f, -0.5f, 0.0f }, { 2.0f, 1.0f }, -2.0f * rotation, color);

		for (float x = -50.0f; x < 50.0f; x += 0.5f)
		{
			for (float y = -50.0f; y < 50.0f; y += 0.5f)
			{
				float r, g, b;
				r = (x + 50.0f) / 100.0f;
				g = (y + 50.0f) / 100.0f;
				b = r * g;
				Pache::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, { r, g, b, 0.5f });
			}
		}
		
		Pache::Renderer2D::endScene();
	}

	void Spell2D::onImGuiRender()
	{
		ImGui::Begin("settings");

		auto stats = Pache::Renderer2D::getStats();
		ImGui::Text("Renderer Stats:");
		ImGui::Text("Draw calls:  %d", stats.drawCalls);
		ImGui::Text("Quad counts: %d", stats.quadCount);
		Pache::Renderer2D::resetStats();
		ImGui::ColorEdit4("color", glm::value_ptr(color));
		ImGui::End();
	}

	void Spell2D::onEvent(Pache::Event& evt)
	{
		cameraController.onEvent(evt);
	}
}