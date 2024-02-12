#pragma once
#include "Core/Layer.h"

namespace Pache
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		void begin();
		void end();
	private:
		float time = 0.0f;
	};
}