#pragma once
#include "Patchouli.h"

namespace Spell
{
	class Spell2D : public Pache::Layer
	{
	public:
		Spell2D();
		virtual ~Spell2D() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(Pache::Timestep ts) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Pache::Event& evt) override;
	private:
		Pache::OrthographicCameraController cameraController;

		Pache::Ref<Pache::VertexArray> vertexArray;
		Pache::Ref<Pache::Shader> shader;
		Pache::Ref<Pache::Texture2D> texture;
		Pache::Ref<Pache::Texture2D> spriteSheet;
		Pache::Ref<Pache::SubTexture2D> stairs;

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}