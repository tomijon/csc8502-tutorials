#include "Renderer.h"
#include <algorithm>
#include <iostream>
#include <utility>

using namespace Render;

Renderer::Renderer(Window& parent) : OGLRenderer(parent), parent(parent) {
	camera = new Camera(-50, 305, {-200, 5, 1.5});
	animator = new CameraAnimator(camera);
	animator->setFade(smoothFade);

	// Frame buffer creation.
	glGenFramebuffers(2, frameBuffers);
	glGenTextures(2, frameTextures);
	glGenTextures(1, &depthTex);

	// Depth texture.
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	// Color Textures.
	for (int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, frameTextures[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	// Attach to render buffer.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[Framebuffer::RENDER]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextures[Framebuffer::RENDER], 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE || !depthTex || !frameTextures[Framebuffer::RENDER]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	// Loading planet textures.
	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/snow_texture.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/snow_bump.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[2] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/mountain_texture.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[3] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/mountain_bump.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[4] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/grass_texture.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[5] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/grass_bump.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[6] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/sand_texture.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[7] = SOIL_load_OGL_texture(TEXTUREDIR"planet/textures/sand_bump.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[8] = SOIL_load_OGL_texture(TEXTUREDIR"sun/textures/lava_texture.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[9] = SOIL_load_OGL_texture(TEXTUREDIR"waterbump.PNG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	textures[10] = SOIL_load_OGL_texture(TEXTUREDIR"sun/textures/lava_bump.JPG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	for (int tex = 0; tex < 11; tex++) {
		if (!textures[tex]) return;
		SetTextureRepeating(textures[tex], true);
	}

	// Loading skybox.
	skyboxMesh = new Quad();
	skybox = SOIL_load_OGL_cubemap(
		TEXTUREDIR"space/nx.png",TEXTUREDIR"space/px.png",
		TEXTUREDIR"space/py.png",TEXTUREDIR"space/ny.png",
		TEXTUREDIR"space/nz.png",TEXTUREDIR"space/pz.png",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	if (!skybox) return;
	skyboxShader = new Shader("skybox_vertex.glsl", "skybox_fragment.glsl");
	if (!skyboxShader->LoadSuccess()) return;


	postProcTex = new Shader("screen_vertex.glsl", "blur_fragment.glsl");
	postProcDraw = new Shader("screen_vertex.glsl", "screen_fragment.glsl");
	screen = new Quad();
	postProcessMesh = new Quad();

	if (!postProcTex || !postProcDraw) return;

	// Spawning the planets in.
	root = new SceneNode();

	Shader* planetShader = new Shader("planet_vertex.glsl", "planet_fragment.glsl");
	Shader* waterShader = new Shader("water_vertex.glsl", "water_fragment.glsl");
	Shader* sunShader = new Shader("planet_vertex.glsl", "sun_fragment.glsl");

	BindShader(planetShader);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "snowHeightStart"), 0.95);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "mountainHeightStart"), 0.7);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "grassHeightStart"), 0.4);

	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "diffusePower"), 0.9);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "specularPower"), 2);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "specularWeight"), 0);
	glUniform1f(glGetUniformLocation(planetShader->GetProgram(), "ambientPower"), 0.1);

	Vector4 shadowColor(0, 0, 0, 1);
	Vector4 ambientColor(1, 0.5, 0, 1);
	Vector4 specularColor(1, 1, 1, 1);

	glUniform4fv(glGetUniformLocation(planetShader->GetProgram(), "shadowColor"), 1, (float*)&shadowColor);
	glUniform4fv(glGetUniformLocation(planetShader->GetProgram(), "ambientColor"), 1, (float*)&ambientColor);
	glUniform4fv(glGetUniformLocation(planetShader->GetProgram(), "specularColor"), 1, (float*)&specularColor);
	glUniform1i(glGetUniformLocation(planetShader->GetProgram(), "timeSkip"), 0);

	BindShader(waterShader);

	glUniform1f(glGetUniformLocation(waterShader->GetProgram(), "diffusePower"), 0.9);
	glUniform1f(glGetUniformLocation(waterShader->GetProgram(), "specularPower"), 32);
	glUniform1f(glGetUniformLocation(waterShader->GetProgram(), "specularWeight"), 1);
	glUniform1f(glGetUniformLocation(waterShader->GetProgram(), "ambientPower"), 0.2);

	glUniform4fv(glGetUniformLocation(waterShader->GetProgram(), "shadowColor"), 1, (float*)&shadowColor);
	glUniform4fv(glGetUniformLocation(waterShader->GetProgram(), "ambientColor"), 1, (float*)&ambientColor);
	glUniform4fv(glGetUniformLocation(waterShader->GetProgram(), "specularColor"), 1, (float*)&ambientColor);

	planet = new Planet(512, 128, planetShader, TEXTUREDIR"planet/");
	water = new Planet(128, 130, waterShader);
	sun = new Planet(32, 512, sunShader);

	planet->AddTexture("snow", textures[0], textures[1]);
	planet->AddTexture("mountain", textures[2], textures[3]);
	planet->AddTexture("grass", textures[4], textures[5]);
	planet->AddTexture("sand", textures[6], textures[7]);

	water->AddTexture("water", 0, textures[9]);
	water->MakeTransparent();

	sun->setPosition(Vector3(500, 100, -40).Normalised() * 2000);
	sun->SetDrawDistance(2000);
	sun->AddTexture("sun", textures[8], textures[10]);

	root->AddChild(planet);
	root->AddChild(water);
	root->AddChild(sun);


	SwitchToPerspective();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	init = true;
}


void Renderer::applyTimeSkip() {
	sun->setScale({ 2, 2, 2 });

	BindShader(planet->GetShader());
	glUniform1i(glGetUniformLocation(planet->GetShader()->GetProgram(), "timeSkip"), 1);
	water->removeMesh();
}


Renderer::~Renderer() {
	//delete shader;
	//delete camera;
	//delete root;
	//delete skyboxShader;

	//glDeleteTextures(8, textures);
}


void Renderer::RenderScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[Framebuffer::RENDER]);

	BuildNodeLists(root);
	SortNodeLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();

	DrawNodes();
	ClearNodeLists();

	Matrix4 temp = viewMatrix;
	
	if (drawPostProc) {
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[Framebuffer::POST_PROC]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, frameTextures[Framebuffer::POST_PROC], 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		BindShader(postProcTex);
		modelMatrix.ToIdentity();
		viewMatrix.ToIdentity();
		projMatrix.ToIdentity();
		UpdateShaderMatrices();

		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(
			postProcTex->GetProgram(), "renderTex"), 0
		);

		const int PASSES = 1;

		int vertical = 0;

		for (int i = 0; i < PASSES; i++) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, frameBuffers[Framebuffer::POST_PROC], 0);

			glUniform1i(glGetUniformLocation(postProcTex->GetProgram(), "isVertical"),
				0);
			glBindTexture(GL_TEXTURE_2D, frameBuffers[Framebuffer::RENDER]);
			postProcessMesh->Draw();

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, frameBuffers[Framebuffer::RENDER], 0);
			glBindTexture(GL_TEXTURE_2D, frameBuffers[Framebuffer::POST_PROC]);
			glUniform1i(glGetUniformLocation(postProcTex->GetProgram(), "isVertical"),
				1);
			glBindTexture(GL_TEXTURE_2D, frameBuffers[Framebuffer::RENDER]);
			postProcessMesh->Draw();
		}
		glEnable(GL_DEPTH_TEST);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(postProcDraw);
	viewMatrix = temp;
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameTextures[Framebuffer::RENDER]);
	glUniform1i(glGetUniformLocation(postProcDraw->GetProgram(), "diffuseTex"), 0);
	screen->Draw();
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

		if (from->GetDrawDistance() <= 0 || from->GetCameraDistance() <= from->GetDrawDistance()) {
			if (from->GetColour().w < 1.0f) {
				transparentNodeList.push_back(from);
			}
			else {
				nodeList.push_back(from);
			}
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
	SwitchToPerspective();
	for (const auto& i : nodeList) {
		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}


void Renderer::DrawNode(SceneNode* node) {
	if (node->GetMesh()) {
		BindShader(node->GetShader());
		UpdateShaderMatrices();
		Vector3 lightDirection = Vector3(500, 100, -40).Normalised() * 2000;
		Vector3 pos = camera->GetPosition();

		glUniform3fv(glGetUniformLocation(node->GetShader()->GetProgram(), "lightPosition"), 1, (float*)&lightDirection);
		glUniform3fv(glGetUniformLocation(node->GetShader()->GetProgram(), "cameraPosition"), 1, (float*)&pos);
		glUniform1f(glGetUniformLocation(node->GetShader()->GetProgram(), "elapsedTime"), (float)parent.GetTimer()->GetTotalTimeSeconds());

		int texCount = 0;
		for (auto iter = node->GetTextureIteratorStart();
				iter != node->GetTextureIteratorEnd(); iter++) {
			if (*iter == 0) continue;
			glActiveTexture(GL_TEXTURE0 + texCount);
			glBindTexture(GL_TEXTURE_2D, *iter);
			texCount += 2;
		}
		int bumpCount = 1;
		for (auto iter = node->GetBumpIteratorStart();
				iter != node->GetBumpIteratorEnd(); iter++) {
			if (*iter == 0) continue;
			glActiveTexture(GL_TEXTURE0 + bumpCount);
			glBindTexture(GL_TEXTURE_2D, *iter);
			bumpCount += 2;
		}

		int texUnit = 0;
		for (auto iter = node->GetNameIteratorStart();
				iter != node->GetNameIteratorEnd(); iter++) {
			std::string name = *iter;
			GLuint texLoc = glGetUniformLocation(node->GetShader()->GetProgram(), (name + "Tex").c_str());
			GLuint bumpLoc = glGetUniformLocation(node->GetShader()->GetProgram(), (name + "Bump").c_str());
			glUniform1i(texLoc, texUnit++);
			glUniform1i(bumpLoc, texUnit++);
		}

		Matrix4 model = node->GetWorldTransform() * Matrix4::Scale(node->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(node->GetShader()->GetProgram(), "modelMatrix"), 1, false, model.values);
		
		node->Draw(*this);
	}
}


void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	Vector3 pos = camera->GetPosition();
	glUniform3fv(glGetUniformLocation(skyboxShader->GetProgram(), "cameraPosition"), 1, (float*)&pos);
	UpdateShaderMatrices();

	skyboxMesh->Draw();

	glDepthMask(GL_TRUE);
}
