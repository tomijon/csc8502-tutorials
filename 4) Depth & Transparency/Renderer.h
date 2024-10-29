#pragma once

#include "nclgl/OGLRenderer.h"
#include "Camera.hpp"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene()				override;
	void UpdateScene(float msec)	override;

	void SwitchToPerspective();
	void SwitchToOrthographic();

	void ToggleObject();
	void ToggleDepth();
	void ToggleAlphaBlend();
	void ToggleBlendMode();
	void MoveObject(float by);

	Matrix4 GetModelMatrix(int mesh);

protected:
	Mesh* meshes[2];
	GLuint textures[2];
	Vector3 positions[2];
	Vector3 scales[2];
	Vector3 rotationAxis[2];
	float rotations[2];

	Shader* shader;
	Camera* camera;

	bool filtering;
	bool repeating;

	bool modifyObject;
	bool usingDepth;
	bool usingAlpha;
	int blendMode;
};

