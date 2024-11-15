#pragma once

#include <queue>

#include "../nclgl/Vector3.h"
#include "Camera.hpp"


float linearFade(double start, double current, double end);
float smoothFade(double start, double current, double end);


template <typename T>
class KeyFrame {
public:
	KeyFrame() : end(0) {}
	KeyFrame(double end, T value) : end(end), value(value) {}

	inline double getEndTime() { return end; }
	inline T getValue() { return value; }

private:
	double end;
	T value;
};


class CameraAnimator {
public:
	CameraAnimator(Camera* camera) : camera(camera), fade(linearFade) {
		pitchTarget = { 0, 0 };
		yawTarget = { 0, 0 };
		rollTarget = { 0, 0 };
	}

	inline void setFade(float (*fadeFunc)(double, double, double)) { fade = fadeFunc; }
	void update(double currentTime, float dt);

	inline void addPitchStep(float pitch, double time) { this->pitch.push({ time, pitch }); pitchEnd = false; }
	inline void addYawStep(float yaw, double time) { this->yaw.push({ time, yaw }); yawEnd = false; }
	inline void addRollStep(float roll, double time) { this->roll.push({ time, roll }); rollEnd = false; }

	inline void setPitchStart(float p) { pitchLast = { 0, p }; pitchTarget = pitchLast; }
	inline void setYawStart(float y) { yawLast = { 0, y }; yawTarget = yawLast; }
	inline void setRollStart(float r) { rollLast = { 0, r }; rollTarget = rollLast; }

	inline void addPositionStep(Vector3 position, double time) { positions.push({ time, position }); positionEnd = false; }
	inline void setPositionStart(Vector3 position) { positionLast = { 0, position }; }

protected:
	void updatePosition(double now);
	void updatePitch(double now);
	void updateYaw(double now);
	void updateRoll(double now);

	std::queue<KeyFrame<Vector3>> positions;
	std::queue<KeyFrame<float>> pitch;
	std::queue<KeyFrame<float>> yaw;
	std::queue<KeyFrame<float>> roll;

	KeyFrame<Vector3> positionTarget;
	KeyFrame<float> pitchTarget;
	KeyFrame<float> yawTarget;
	KeyFrame<float> rollTarget;

	KeyFrame<Vector3> positionLast;
	KeyFrame<float> pitchLast;
	KeyFrame<float> yawLast;
	KeyFrame<float> rollLast;

	bool positionEnd = true;
	bool pitchEnd = true;
	bool yawEnd = true;
	bool rollEnd = true;

	float (*fade)(double, double, double);
	Camera* camera;
};


template <typename T>
T lerp(T x, T y, float weight) { return x + ((y - x) * weight); }
