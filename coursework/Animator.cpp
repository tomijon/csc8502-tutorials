#include "Animator.hpp"


void CameraAnimator::update(double now) {
	updatePosition(now);
	updatePitch(now);
	updateYaw(now);
	updateRoll(now);

	float weight;

	weight = fade(positionLast.getEndTime(), now, positionTarget.getEndTime());
	Vector3 newPos = lerp<Vector3>(positionLast.getValue(), positionTarget.getValue(), weight);
	camera->SetPosition(newPos);

	weight = fade(pitchLast.getEndTime(), now, pitchTarget.getEndTime());
	float newPitch = lerp<float>(pitchLast.getValue(), pitchTarget.getValue(), weight);
	camera->SetPitch(newPitch);

	weight = fade(yawLast.getEndTime(), now, yawTarget.getEndTime());
	float newYaw = lerp<float>(yawLast.getValue(), yawTarget.getValue(), weight);
	camera->SetYaw(newYaw);

	weight = fade(rollLast.getEndTime(), now, rollTarget.getEndTime());
	float newRoll = lerp<float>(rollLast.getValue(), rollTarget.getValue(), weight);
	camera->SetRoll(newRoll);
}


void CameraAnimator::updatePosition(double now) {
	if (now > positionTarget.getEndTime() && !positions.empty()) {
		positionLast = positionTarget;
		positionTarget = positions.front();
		positions.pop();
	}
}


void CameraAnimator::updatePitch(double now) {
	if (now > pitchTarget.getEndTime() && !pitch.empty()) {
		pitchLast = pitchTarget;
		pitchTarget = pitch.front();
		pitch.pop();
	}
}


void CameraAnimator::updateYaw(double now) {
	if (now > yawTarget.getEndTime() && !yaw.empty()) {
		yawLast = yawTarget;
		yawTarget = yaw.front();
		yaw.pop();
	}
}


void CameraAnimator::updateRoll(double now) {
	if (now > rollTarget.getEndTime() && !roll.empty()) {
		rollLast = rollTarget;
		rollTarget = roll.front();
		roll.pop();
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
