#pragma once

namespace Pache
{
	class Input
	{
	public:
		static bool isKeyPressed(int keycode) { return instance->isKeyPressedImpl(keycode); }
		static bool isMouseButtonPressed(int button) { return instance->isMouseButtonPressedImpl(button); }
		static float getMouseX() { return instance->getMouseXImpl(); }
		static float getMouseY() { return instance->getMouseYImpl(); }
		static std::pair<float, float> getMousePos() { return instance->getMousePosImpl(); }
	protected:
		virtual bool isKeyPressedImpl(int keycode) const = 0;
		virtual bool isMouseButtonPressedImpl(int button) const = 0;
		virtual float getMouseXImpl() const = 0;
		virtual float getMouseYImpl() const = 0;
		virtual std::pair<float, float> getMousePosImpl() const = 0;
	private:
		static Input* instance;
	};
}