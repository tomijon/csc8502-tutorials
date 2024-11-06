#include "Renderer.h"
#include "shapes.hpp"
#include <algorithm>

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	heightMap = new HeightMap(TEXTUREDIR"noise.png");
	camera = new Camera(-40, 270, Vector3());
	
	Vector3 dimensions = heightMap->GetHeightmapSize();
	camera->SetPosition(dimensions * Vector3(0.5, 2, 0.5));
	
	shader = new Shader("tut8_vertex.glsl ",
			"tut8_fragment.glsl");
	if (!shader->LoadSuccess()) {
		return;
	}
	
	terrainTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG",
			SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!terrainTex) {
		return;
	}
	
	SetTextureRepeating(terrainTex,true);
	SwitchToPerspective();
	
	root = new SceneNode();
	root->SetMesh(heightMap);
	root->SetTexture(terrainTex);
	root->SetBoundingRadius(std::max(dimensions.x, dimensions.z) * 16);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	init = true;
}


Renderer::~Renderer() {
	delete shader;
	delete camera;
	delete root;
	delete heightMap;
	glDeleteTextures(1, &terrainTex);
}


void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	BindShader(shader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);

	DrawNodes();
	ClearNodeLists();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, terrainTex);
	//heightMap->Draw();
}


void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(msec);
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1, 10000, (float)width / (float)height, 45);
}


void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
		camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));
		
		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}
	for (vector < SceneNode* >::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i){
	BuildNodeLists((*i));
	}
}


void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}


void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {
		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}


void Renderer::DrawNode(SceneNode* node) {
	if (node->GetMesh()) {
		Matrix4 model = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, model.values);
		//glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&node->GetColour());

		GLuint texture = node->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		node->Draw(*this);
	}
}
