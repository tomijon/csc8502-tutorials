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
	void doBufferStuff() { GenerateNormals(); GenerateTangents(); BufferData(); }

private:
	Vector3 up;
	Vector3 axis0;
	Vector3 axis1;

	int size;
};


class Planet : public SceneNode {
public:
	Planet(int size, int radius, Shader* shader, std::string noise = "");
	~Planet() { delete[] sides; }

	virtual void Update(float dt) override;
	void AddTexture(std::string uniformName, GLuint texture, GLuint bump = 0) override {
		for (auto child = GetChildIteratorStart(); child != GetChildIteratorEnd(); child++) {
			(*child)->AddTexture(uniformName, texture, bump);
		}
	}

	void setPosition(Vector3 p) { position = p; }
	Vector3 getPosition() const { return position; }

	void MakeTransparent() {
		for (SceneNode* child : children) {
			child->SetColour({ 0, 0, 0, 0.8 });
		}
	}

	void removeMesh() {
		for (SceneNode* child : children) {
			delete child->GetMesh();
			child->SetMesh(0);
		}
	}

	void setScale(Vector3 s) { scale = s; }

private:
	PlanetSurface* sides[6];

	Vector3 position;
	Vector3 scale;
	float yaw = 0;
};
