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

	void ToggleScissor();
	void ToggleStencil();

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

	bool usingScissor;
	bool usingStencil;
};

