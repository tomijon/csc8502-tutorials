#include <iostream>
#include "Camera.hpp"
#include "../nclgl/Window.h"

void Camera::UpdateCamera(float dt) {
	UpdateRotation();
	UpdatePosition(dt);
}


void Camera::UpdateRotation() {
	pitch -= Window::GetMouse()->GetRelativePosition().y;
	yaw -= Window::GetMouse()->GetRelativePosition().x;

	pitch = std::max(std::min(pitch, 90.0f), -90.0f);

	if (yaw < 0) yaw += 360;
	if (yaw > 360) yaw -= 360;
}


void Camera::UpdatePosition(float dt) {
	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) position += forward * speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) position -= forward * speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) position -= right * speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) position += right * speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) position.y -= speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) position.y += speed * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) roll -= 10 * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) roll += 10 * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_F)) {
		std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
		std::cout << "Pitch: " << pitch << std::endl;
		std::cout << "Yaw: " << yaw << std::endl;
		std::cout << "Roll: " << roll << std::endl;
	}
}


Matrix4 Camera::BuildViewMatrix() {
	return Matrix4::Rotation(-roll, Vector3(0, 0, 1)) *
		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
}
