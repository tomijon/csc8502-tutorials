#include <unordered_map>

#include "../nclgl/Vector2.h"
#include "../nclgl/Mesh.h"

#include "terrain.hpp"

using namespace Terrain;

NoiseGenerator::NoiseGenerator(Vector2 dimensions) {
	width = static_cast<unsigned int>(dimensions.x);
	height = static_cast<unsigned int>(dimensions.y);

	freq = 1.0f;
	amp = 1.0f;
	freqMult = 2.0f;
	ampMult = 0.5f;

	layers = 1;
	randomSeed = 0;

	image = new float[width * height];
	gradients = new Vector2[(width + 1) * (height + 1)];
}


NoiseGenerator::~NoiseGenerator() {
	delete image;
	delete gradients;
}


void NoiseGenerator::create() {
	generate();
	applyDistance();
}


Vector2 NoiseGenerator::getGradient(int x, int y) {
	Vector2 gradient = gradients[pointToIndex(x, y)];
	return (gradient.x == 0 and gradient.y == 0) ? generateGradient(x, y) : gradient;
}


float NoiseGenerator::lerp(float low, float high, float weight) {
	return low + ((high - low) * weight);
}


int NoiseGenerator::pointToIndex(int x, int y) {
	return (y * width) + x;
}


float NoiseGenerator::fade(float val) {
	return (6 * std::pow(val, 5)) - (15 * std::pow(val, 4)) + (10 * std::pow(val, 3);
}


void NoiseGenerator::resetGradients() {
	for (int i = 0; i < (width + 1) * (height + 1); i++) {
		gradients[i].ToZero();
	}
}










float Terrain::dot(const Vector2& a, const Vector2& b) {
	return (a.x * b.x) + (a.y * b.y);
}


Vector2 Terrain::normalise(const Vector2& vector) {
	float mag = std::sqrt(dot(vector, vector));
	return mag == 0 ? Vector2() : Vector2(vector.x / mag, vector.y / mag);
}






