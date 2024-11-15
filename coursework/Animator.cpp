#include "Animator.hpp"
#include "../nclgl/Window.h"


void CameraAnimator::update(double now, float dt) {
	updatePosition(now);
	updatePitch(now);
	updateYaw(now);
	updateRoll(now);

	float weight;
	
	if (!positionEnd) {
		weight = fade(positionLast.getEndTime(), now, positionTarget.getEndTime());
		Vector3 newPos = lerp<Vector3>(positionLast.getValue(), positionTarget.getValue(), weight);
		camera->SetPosition(newPos);
	}

	if (!pitchEnd) {
		weight = fade(pitchLast.getEndTime(), now, pitchTarget.getEndTime());
		float newPitch = lerp<float>(pitchLast.getValue(), pitchTarget.getValue(), weight);
		camera->SetPitch(newPitch);
	}

	if (!yawEnd) {
		weight = fade(yawLast.getEndTime(), now, yawTarget.getEndTime());
		float newYaw = lerp<float>(yawLast.getValue(), yawTarget.getValue(), weight);
		camera->SetYaw(newYaw);
	}

	if (!rollEnd) {
		weight = fade(rollLast.getEndTime(), now, rollTarget.getEndTime());
		float newRoll = lerp<float>(rollLast.getValue(), rollTarget.getValue(), weight);
		camera->SetRoll(newRoll);
	}

	if (!(positionEnd && pitchEnd && yawEnd && rollEnd)) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
			positionEnd = true;
			pitchEnd = true;
			yawEnd = true;
			rollEnd = true;
		}
	}
	else {
		camera->UpdateCamera(dt);
	}
}


void CameraAnimator::updatePosition(double now) {
	if (now > positionTarget.getEndTime()) {
		positionLast = positionTarget;

		if (!positions.empty()) {
			positionTarget = positions.front();
			positions.pop();
		}
		else {
			positionEnd = true;
		}
	}
	
}


void CameraAnimator::updatePitch(double now) {
	if (now > pitchTarget.getEndTime()) {
		pitchLast = pitchTarget;

		if (!pitch.empty()) {
			pitchTarget = pitch.front();
			pitch.pop();
		}
		else {
			pitchEnd = true;
		}
	}
}


void CameraAnimator::updateYaw(double now) {
	if (now > yawTarget.getEndTime()) {
		yawLast = yawTarget;

		if (!yaw.empty()) {
			yawTarget = yaw.front();
			yaw.pop();
		}
		else {
			yawEnd = true;
		}
	}
}


void CameraAnimator::updateRoll(double now) {
	if (now > rollTarget.getEndTime()) {
		rollLast = rollTarget;

		if (!roll.empty()) {
			rollTarget = roll.front();
			roll.pop();
		}
		else {
			rollEnd = true;
		}
	}
}


float linearFade(double start, double current, double end) {
	return static_cast<float>(current - start) / static_cast<float>(end - start);
}


float smoothFade(double start, double current, double end) {
	float weight = linearFade(start, current, end);
	if (weight < 0.5) return 2 * weight * weight;
	return (-2 * (weight - 1) * (weight - 1)) + 1;
}
