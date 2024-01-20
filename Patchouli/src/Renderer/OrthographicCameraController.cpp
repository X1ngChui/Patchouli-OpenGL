#include "Renderer/OrthographicCameraController.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Pache
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotationEnabled)
		: aspectRatio(aspectRatio), camera(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel), rotationEnabled(rotationEnabled)
	{
	}

	void OrthographicCameraController::onUpdate(Timestep timestep)
	{
		if (Input::isKeyPressed(PACHE_KEY_A))
			position.x -= translationSpeed * timestep * zoomLevel;
		else if (Input::isKeyPressed(PACHE_KEY_D))
			position.x += translationSpeed * timestep * zoomLevel;
		if (Input::isKeyPressed(PACHE_KEY_W))
			position.y += translationSpeed * timestep * zoomLevel;
		else if (Input::isKeyPressed(PACHE_KEY_S))
			position.y -= translationSpeed * timestep * zoomLevel;

		camera.setPosition(position);

		if (rotationEnabled)
		{
			if (Input::isKeyPressed(PACHE_KEY_Q))
				rotation += rotationSpeed * timestep;
			else if (Input::isKeyPressed(PACHE_KEY_E))
				rotation -= rotationSpeed * timestep;

			camera.setRotation(rotation);
		}
	}

	void OrthographicCameraController::onEvent(Event& evt)
	{
		EventDispatcher dispatcher(evt);
		dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& evt)
			{
				return this->onMouseScrolled(evt);
			});
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& evt)
			{
				return this->onWindowResized(evt);
			});
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& evt)
	{
		zoomLevel -= evt.getOffsetY() * 0.25f;
		zoomLevel = std::max(zoomLevel, 0.25f);
		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& evt)
	{
		aspectRatio = (float)evt.getWidth() / (float)evt.getHeight();
		camera.setProjection(-aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel);
		return false;
	}
}