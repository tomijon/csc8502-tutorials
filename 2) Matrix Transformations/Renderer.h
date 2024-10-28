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

	void SetFOV(float fov);

	Vector3 GetPosition() { return camera->GetPosition(); }

	inline void SetScale(float s) { scale = s; };
	inline void SetRotation(float r) { rotation = r; };
protected:
	Mesh* triangle; 
	Shader* basicShader;
	Camera* camera;

	float scale = 1;
	float rotation = 1;
	float fov = 45;
};

