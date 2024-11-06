#pragma once

#include "nclgl/Mesh.h"
#include "SceneGraph.hpp"

class Triangle : public Mesh {
public:
	Triangle();
	~Triangle() {};
};


class Quad : public Mesh {
public:
	Quad();
	~Quad() {};
};


class CubeRobot : public SceneNode {
public:
	CubeRobot(Mesh* cube);
	~CubeRobot(void) {};
	void Update(float dt) override;

protected:
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
};


class Plane {
public:
	Plane(void) {};
	Plane(const Vector3& normal, float distance, bool normalise = false);
	~Plane(void) {};

	void SetNormal(const Vector3& normal) { this->normal = normal; }
	Vector3 GetNormal() const { return normal; }

	void SetDistance(float dist) { distance = dist; }
	float GetDistance() const { return distance; }

	bool SphereInPlane(const Vector3& position, float radius) const;

protected:
	Vector3 normal;
	float distance;
};


class Frustum {
public:
	Frustum(void) {};
	~Frustum(void) {};

	void FromMatrix(const Matrix4& mvp);
	bool InsideFrustum(SceneNode& node);

protected:
	Plane planes[6];
};