#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Window.h"
#include "Camera.hpp"
#include "SceneGraph.hpp"
#include "shapes.hpp"
#include "Animator.hpp"
#include "Planet.hpp"


namespace Render {
	enum Framebuffer {
		RENDER = 0,
		POST_PROC = 1
	};
}

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
	Shader* postProcDraw;
	Shader* postProcTex;
	Camera* camera;
	CameraAnimator* animator;
	SceneNode* root;
	GLuint textures[11];
	GLuint skybox;
	Mesh* skyboxMesh;
	Mesh* postProcessMesh;
	Mesh* screen;

	GLuint frameBuffers[2];
	GLuint frameTextures[2];
	GLuint depthTex;
	bool drawPostProc = true;

	Planet* planet;
	Planet* water;
	Planet* sun;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

