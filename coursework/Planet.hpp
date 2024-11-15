#pragma once

#include <string>

#include "../nclgl/Vector3.h"
#include "../nclgl/Mesh.h"

class PlanetSurface : public Mesh {
public:
	PlanetSurface(Vector3 up, int size);

	void generateMesh();
	void applyNoise(std::string name);



private:
	Vector3 up;
	Vector3 axis0;
	Vector3 axis1;

	int size;
};
