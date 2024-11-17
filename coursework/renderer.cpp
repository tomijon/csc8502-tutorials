#include "Renderer.h"
#include "terrain.hpp"
#include <algorithm>
#include <iostream>

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	//Terrain::NoiseGenerator generator({ 1024, 1024 });
	//generator.setFreq(1.0f / 64.0f);
	//generator.setLayers(1);
	//generator.setAmpMult(0.5);
	//generator.setFreqMult(2);
	//generator.create();

	//heightMap = new Terrain::Heightmap(generator);
	//surf->applyNoise("out.png");
	camera = new Camera(-50, 305, {-200, 5, 1.5});
	animator = new CameraAnimator(camera);
	//animator->setFade(smoothFade);

	//animator->setPositionStart({ 5886.7, 3473.02, 79.0327 });
	//animator->setPitchStart(-21.59);
	//animator->setYawStart(203.57);
	//animator->setRollStart(0);

	//animator->addPositionStep({ 12082.1, 1936.93, 913.073 }, 15);
	//animator->addPositionStep({ 17007.7, 4550.44, 3224.58 }, 20);

	//animator->addPitchStep(-7.59005, 10);
	//animator->addPitchStep(-25.44, 25);

	//animator->addYawStep(155.757, 12);
	//animator->addYawStep(114.035, 22);

	//animator->addRollStep(0, 14);
	//animator->addRollStep(-65, 25);

	//Vector3 dimensions = { 1024, 1024, 1024 };
	//camera->SetPosition(dimensions * Vector3(0.5, 2, 0.5));
	
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

	quad = new Quad();
	skybox = SOIL_load_OGL_cubemap(
		TEXTUREDIR"space/nx.png",TEXTUREDIR"space/px.png",
		TEXTUREDIR"space/py.png",TEXTUREDIR"space/ny.png",
		TEXTUREDIR"space/nz.png",TEXTUREDIR"space/pz.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	if (!skybox) return;
	skyboxShader = new Shader("skybox_vertex.glsl", "skybox_fragment.glsl");
	if (!skyboxShader->LoadSuccess()) return;
	
	SetTextureRepeating(terrainTex,true);
	SwitchToPerspective();
	
	root = new SceneNode();
	root->AddChild(new Planet(2048, 128, TEXTUREDIR"out.png"));
	root->AddChild(new Planet(1024, 129));


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	init = true;
}


Renderer::~Renderer() {
	delete shader;
	delete camera;
	delete root;
	delete skyboxShader;

	glDeleteTextures(1, &terrainTex);
}


void Renderer::RenderScene() {
	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	
	BindShader(shader);
	UpdateShaderMatrices();
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	Vector3 lightDirection = Vector3(500, 100, -40);
	lightDirection.Normalise();
	Vector3 pos = camera->GetPosition();
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "lightDirection"), 1, (float*)&lightDirection);
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPosition"), 1, (float*)&pos);


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
	//camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();

	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(msec);
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1, 20000, (float)width / (float)height, 45);
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
		Vector4 color = node->GetColour();
		glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&color);

		GLuint texture = node->GetTexture();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		node->Draw(*this);
	}
}


void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);
}
