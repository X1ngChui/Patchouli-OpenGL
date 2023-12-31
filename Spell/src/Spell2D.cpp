#include "Spell2D.h"
#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Spell
{
	Spell2D::Spell2D()
		: Layer("Spell 2D"), cameraController(1280.0f / 720.0f)
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
		cameraController.onUpdate(ts);

		Pache::RenderCommand::setClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Pache::RenderCommand::clear();

		Pache::Renderer2D::beginScene(cameraController.getCamera());

		Pache::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 5.0f, 5.0f }, texture);
		Pache::Renderer2D::drawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, color);
		
		Pache::Renderer2D::endScene();
	}

	void Spell2D::onImGuiRender()
	{
		ImGui::Begin("settings");
		ImGui::ColorEdit4("color", glm::value_ptr(color));
		ImGui::End();
	}

	void Spell2D::onEvent(Pache::Event& evt)
	{
		cameraController.onEvent(evt);
	}
}