#include "MeCamera.h"
#include <glm\gtx\transform.hpp>

const float MeCamera::MOVEMENT_SPEED = 0.1f;
MeCamera::MeCamera() : UP(0.0f, 1.0f, 0.0f)
{
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}


MeCamera::~MeCamera()
{
}

glm::mat4 MeCamera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void MeCamera::mouseUpdate(const glm::vec2 & newMousePosition)
{

	strafeDirection = glm::cross(viewDirection, UP);
	glm::mat4 rotator = glm::rotate(newMousePosition.x, UP) *

		glm::rotate(newMousePosition.y, strafeDirection);
	viewDirection = glm::mat3(rotator) *viewDirection;


	viewDirection = glm::mat3() * viewDirection;
}

void MeCamera::setViewDirection(glm::vec3 newViewDirection)
{
	viewDirection = newViewDirection;
}

glm::vec3 MeCamera::getViewDirection()
{
	return viewDirection;
}

glm::vec3 MeCamera::getUp()
{
	return UP;
}

void MeCamera::moveForward() {
	position += MOVEMENT_SPEED * viewDirection;
}
void MeCamera::moveBackward() {
	position -= MOVEMENT_SPEED * viewDirection;
}
void MeCamera::strafeLeft() {
	position -= MOVEMENT_SPEED * strafeDirection;
}
void MeCamera::strafeRight() {
	position += MOVEMENT_SPEED * strafeDirection;
}
void MeCamera::moveUP() {
	position += MOVEMENT_SPEED * UP;
}
void MeCamera::moveDown() {
	position -= MOVEMENT_SPEED * UP;
}

void MeCamera::reset()
{
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}