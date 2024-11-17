#pragma once

#include "../nclgl/OGLRenderer.h"
#include "Camera.hpp"
#include "SceneGraph.hpp"
#include "shapes.hpp"
#include "Animator.hpp"
#include "Planet.hpp"

#include "terrain.hpp"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene()				override;
	void UpdateScene(float msec)	override;
	CameraAnimator* getAnimator() { return animator; }

	void SwitchToPerspective();

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* node);
	void DrawSkybox();

	Shader* shader;
	Shader* skyboxShader;
	Camera* camera;
	CameraAnimator* animator;
	SceneNode* root;
	GLuint terrainTex;
	GLuint skybox;
	PlanetSurface* surf;
	Mesh* quad;


	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

