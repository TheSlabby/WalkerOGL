#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera
{
private:
	glm::vec2 cameraAngle;
	glm::vec3 getFrontVector();
	glm::vec2 lastMousePos = glm::vec2(400.0f,300.0f);
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);

public:
	void updateMouse(glm::vec2 mousePos);
	void updatePos(glm::vec3 moveDirection);
	void print();
	glm::mat4 getViewMatrix();
};