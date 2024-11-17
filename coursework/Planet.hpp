#pragma once

#include <string>

#include "../nclgl/Vector3.h"
#include "../nclgl/Mesh.h"
#include "SceneGraph.hpp"

class PlanetSurface : public Mesh {
public:
	PlanetSurface() {}
	PlanetSurface(Vector3 up, int size);

	void setUpVector(Vector3 up) { this->up = up; }
	void setSize(int size) { this->size = size; }

	void generateMesh();
	void setRadius(float radius);
	void applyNoise(std::string name, float radius);
	void doBufferStuff() { GenerateNormals(); BufferData(); }

private:
	Vector3 up;
	Vector3 axis0;
	Vector3 axis1;

	int size;
};


class Planet : public SceneNode {
public:
	Planet(int size, int radius, std::string noise = "");
	~Planet() { delete[] sides; }

	virtual void Update(float dt) override;

private:
	PlanetSurface* sides[6];

	Vector3 position;
	Vector3 scale;
	float yaw = 0;
};
