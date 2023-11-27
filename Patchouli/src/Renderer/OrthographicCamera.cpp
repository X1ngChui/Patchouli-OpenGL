#include "Renderer/OrthographicCamera.h"

namespace Pache
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), view(1.0f), position(0.0f), rotation(0.0f)
	{
		viewProjection = projection * view;
	}

	void OrthographicCamera::resetMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		view = glm::inverse(transform);
		viewProjection = projection * view;
	}
}