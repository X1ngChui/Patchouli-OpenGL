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
		spriteSheet = Pache::Texture2D::create("assets/textures/rpg_sheet.png");
		stairs = Pache::SubTexture2D::create(spriteSheet, { 7, 6 }, { 128, 128 });
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

		// Pache::Renderer2D::drawQuad({ 1.0f, 1.0f }, { 0.5f, 0.5f }, color);
		// Pache::Renderer2D::drawQuad({ -0.6f, -0.6f }, { 0.8f, 0.8f }, texture);
		// Pache::Renderer2D::drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, spriteSheet);
		Pache::Renderer2D::drawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, stairs);
		
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