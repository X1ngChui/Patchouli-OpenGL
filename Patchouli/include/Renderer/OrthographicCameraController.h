#pragma once

namespace Pache
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool roationEabled = false);

		OrthographicCamera& getCamera() { return camera; }
		const OrthographicCamera& getCamera() const { return camera; }

		void setZoomLevel(float zoomLevel) { this->zoomLevel = zoomLevel; }
		float getZoomLevel() const { return zoomLevel; }

		void onUpdate(Timestep timestep);
		void onEvent(Event& evt);
	private:
		bool onMouseScrolled(MouseScrolledEvent& evt);
		bool onWindowResized(WindowResizeEvent& evt);
	private:
		float aspectRatio;
		float zoomLevel = 1.0f;

		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		float translationSpeed = 5.0f;
		float rotation = 0.0f;
		float rotationSpeed = 3.14f;
		OrthographicCamera camera;

		bool rotationEnabled;
	};
}