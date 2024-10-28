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

	void UpdateTextureMatrix(float rotation);
	void ToggleRepeating();
	void ToggleFiltering();

protected:
	Mesh* triangle; 
	Shader* basicShader;
	Camera* camera;

	GLuint texture;
	bool filtering;
	bool repeating;
};

