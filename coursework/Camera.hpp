#pragma once

#include "../nclgl/Matrix4.h"
#include "../nclgl/Vector3.h"

class Camera {
public:
	Camera(void) : pitch(0), yaw(0), roll(0), speed(20) {};

	Camera(float pitch, float yaw, Vector3 position) : pitch(pitch), yaw(yaw), roll(0), speed(20), position(position) {};

	~Camera(void) {};


	void UpdateCamera(float dt = 1);
	
	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 pos) {	position = pos;	}

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }

	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetRoll() const { return roll; }
	void SetRoll(float r) { roll = r; }

	float GetSpeed() const { return speed; }
	void SetSpeed(float s) { speed = s; }

protected:
	float pitch;
	float yaw;
	float roll;
	float speed;
	Vector3 position;

private:
	void UpdateRotation();
	void UpdatePosition(float dt);
};