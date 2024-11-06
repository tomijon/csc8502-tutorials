#pragma once

#include "nclgl/OGLRenderer.h"
#include "Camera.hpp"
#include "SceneGraph.hpp"
#include "shapes.hpp"

class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);

	void RenderScene()				override;
	void UpdateScene(float msec)	override;

	void SwitchToPerspective();

protected:
	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* node);

	Shader* shader;
	Camera* camera;
	SceneNode* root;
	Mesh* cube;
	Mesh* quad;
	GLuint texture;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};

