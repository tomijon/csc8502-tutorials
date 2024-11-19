#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Window.h"
#include "Camera.hpp"
#include "SceneGraph.hpp"
#include "shapes.hpp"
#include "Animator.hpp"
#include "Planet.hpp"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene()				override;
	void UpdateScene(float msec)	override;
	CameraAnimator* getAnimator() { return animator; }

	void SwitchToPerspective();
	void applyTimeSkip();

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* node);
	void DrawSkybox();

	Window& parent;

	Shader* shader;
	Shader* skyboxShader;
	Camera* camera;
	CameraAnimator* animator;
	SceneNode* root;
	GLuint textures[11];
	GLuint skybox;
	Mesh* quad;

	Planet* planet;
	Planet* water;
	Planet* sun;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

