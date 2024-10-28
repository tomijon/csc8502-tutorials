#pragma once

#include "nclgl/OGLRenderer.h"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene()				override;
	void UpdateScene(float msec)	override;

	void SwitchToPerspective();
	void SwitchToOrthographic();

	void SetFOV(float fov);

	inline void SetScale(float s) { scale = s; };
	inline void SetRotation(float r) { rotation = r; };
	inline void SetPosition(Vector3 v) { position = v; };
protected:
	Mesh* triangle; 
	Shader* basicShader;

	float scale = 1;
	float rotation = 1;
	float fov = 45;
	Vector3 position;
};

