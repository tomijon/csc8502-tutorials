#include <limits>

#include "../nclgl/Vector2.h"
#include "../nclgl/Mesh.h"

#include "terrain.hpp"
#include <cstdlib>

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
	for (int i = 0; i < width * height; i++) {
		image[i] = 0;
	}
}


NoiseGenerator::~NoiseGenerator() {
	delete image;
}


void NoiseGenerator::create() {
	generate();
	applyDistance();
}


Vector2 NoiseGenerator::getGradient(int x, int y) {
	Vector2 gradient = gradients[Point(x, y)];
	return (gradient.x == 0 and gradient.y == 0) ? generateGradient(x, y) : gradient;
}


float NoiseGenerator::lerp(float low, float high, float weight) const {
	return low + ((high - low) * weight);
}


int NoiseGenerator::pointToIndex(int x, int y) const {
	return (y * width) + x;
}


float NoiseGenerator::fade(float val) const {
	return (6 * std::pow(val, 5)) - (15 * std::pow(val, 4)) + (10 * std::pow(val, 3));
}


void NoiseGenerator::resetGradients() {
	gradients.clear();
}


void NoiseGenerator::generate() {
	srand(randomSeed);
	int currentIndex;
	float currentHeight;
	float lowest = std::numeric_limits<float>::max();
	float highest = std::numeric_limits<float>::min();

	for (int l = 0; l < layers; l++) {
		resetGradients();

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				currentIndex = pointToIndex(x, y);
				currentHeight = amp * (1 + calculateHeight((float)x * freq, (float)y * freq)) / 2.0f;
				image[currentIndex] += currentHeight;
				
				if (image[currentIndex] < lowest) lowest = image[currentIndex];
				if (image[currentIndex] > highest) highest = image[currentIndex];
			}
		}

		freq *= freqMult;
		amp *= ampMult;
	}
	normalise(lowest, highest);
}


float NoiseGenerator::calculateHeight(float x, float y) {
	float x0 = std::floor(x);
	float x1 = x0 + 1;
	float y0 = std::floor(y);
	float y1 = y0 + 1;

	float dx = x - x0;
	float dy = y - y0;

	Vector2 corner0 = getGradient(x0, y0);
	Vector2 corner1 = getGradient(x1, y0);
	Vector2 corner2 = getGradient(x0, y1);
	Vector2 corner3 = getGradient(x1, y1);

	Vector2 off0 = { dx, dy };
	Vector2 off1 = { 1 - dx, dy };
	Vector2 off2 = { dx, 1 - dy };
	Vector2 off3 = { 1 - dx, 1 - dy };

	float dot0 = dot(corner0, off0);
	float dot1 = dot(corner1, off1);
	float dot2 = dot(corner2, off2);
	float dot3 = dot(corner3, off3);

	float fadeX = fade(dx);
	float fadeY = fade(dy);

	float lerp0 = lerp(dot0, dot1, fadeX);
	float lerp1 = lerp(dot2, dot3, fadeX);
	return lerp(lerp0, lerp1, fadeY);
}


Vector2 NoiseGenerator::generateGradient(int x, int y) {
	Point current = { x, y };

	float angle = (rand() / (float)RAND_MAX) * 2 * PI;
	Vector2 vector = Terrain::normalise(Vector2(cos(angle), sin(angle)));
	gradients[current] = vector;
	return vector;
}


void NoiseGenerator::normalise(float lowest, float highest) {
	float difference = highest - lowest;
	for (int i = 0; i < width * height; i++) {
		image[i] -= lowest;
		image[i] /= difference;
	}
}


void NoiseGenerator::applyDistance() {
	for (int x = 0; x < width; x++) {
		for (int z = 0; z < height; z++) {
			float distance = getDistance(x, z);
			image[pointToIndex(x, z)] *= 1 - (distance / (std::min(width, height) / 2.0f));
		}
	}
}


float NoiseGenerator::getDistance(int x, int z) {
	float middleX = width / 2.0f;
	float middleZ = height / 2.0f;
	return std::sqrt(std::pow(x - middleX, 2) + std::powl(z - middleZ, 2));
}


Heightmap::Heightmap(const NoiseGenerator& noise) {
	numVertices = noise.getWidth() * noise.getHeight();
	numIndices = (noise.getWidth() - 1) * (noise.getHeight() - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	Vector3 vertexScale = Vector3(16.0f, 1.0f, 16.0f);
	Vector2 textureScale = Vector2(1.0f / 16.0f, 1.0f / 16.0f);

	for (int z = 0; z < noise.getHeight(); z++) {
		for (int x = 0; x < noise.getWidth(); x++) {
			int index = noise.pointToIndex(x, z);
			vertices[index] = Vector3(x, (1 << 12) * noise.getNoise()[index], z) * vertexScale;
			textureCoords[index] = Vector2(x, z) * textureScale;
		}
	}

	int i = 0;
	for (int z = 0; z < noise.getHeight() - 1; z++) {
		for (int x = 0; x < noise.getWidth() - 1; x++) {
			int a = (z * noise.getWidth()) + x;
			int b = (z * noise.getWidth()) + (x + 1);
			int c = ((z + 1) * noise.getWidth()) + (x + 1);
			int d = ((z + 1) * noise.getWidth()) + x;

			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;

			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}
	BufferData();

	heightmapSize.x = vertexScale.x * (noise.getWidth() - 1);
	heightmapSize.y = vertexScale.y * 255.0f;
	heightmapSize.x = vertexScale.z * (noise.getHeight() - 1);
}




float Terrain::dot(const Vector2& a, const Vector2& b) {
	return (a.x * b.x) + (a.y * b.y);
}


Vector2 Terrain::normalise(const Vector2& vector) {
	float mag = std::sqrt(dot(vector, vector));
	return mag == 0 ? Vector2() : Vector2(vector.x / mag, vector.y / mag);
}






