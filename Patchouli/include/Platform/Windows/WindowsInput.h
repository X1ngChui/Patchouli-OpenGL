#pragma once
#include "Core/Input.h"

namespace Pache
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool isKeyPressedImpl(int keycode) const override;
		virtual bool isMouseButtonPressedImpl(int button) const override;
		virtual float getMouseXImpl() const override;
		virtual float getMouseYImpl() const override;
		virtual std::pair<float, float> getMousePosImpl() const override;
	};
}