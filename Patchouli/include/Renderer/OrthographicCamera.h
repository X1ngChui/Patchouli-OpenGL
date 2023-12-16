#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Pache
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void setProjection(float left, float right, float bottom, float top);

		void setPosition(const glm::vec3& position) { this->position = position; resetMatrix(); }
		const glm::vec3& getPosition() const { return position; }

		void setRotation(float rotation) { this->rotation = rotation; resetMatrix(); }
		float getRotation() const { return rotation; }

		const glm::mat4& getProjection() { return projection; }
		const glm::mat4& getView() { return view; }
		const glm::mat4& getViewProjection() { return viewProjection; }
	private:
		void resetMatrix();
	private:
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 viewProjection;

		glm::vec3 position;
		float rotation;
	};
}