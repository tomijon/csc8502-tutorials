#include "Renderer.h"
#include "shapes.hpp"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	meshes[0] = new Triangle;
	meshes[1] = new Quad;
	camera = new Camera(0, 0, { 0, 0, 0 });

	shader = new Shader("tut3_vertex.glsl", "tut3_fragment.glsl");
	if (!shader->LoadSuccess()) return;

	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!textures[0]) return;
	
	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR"stainedglass.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!textures[1]) return;

	positions[0] = { 0, 0, -5 };
	positions[1] = { 0, 0, 0 };

	scales[0] = { 10, 10, 10 };
	scales[1] = { 10, 10, 10 };

	rotations[0] = 0;
	rotations[1] = 0;

	usingDepth = false;
	usingAlpha = false;
	blendMode = 0;
	modifyObject = true;

	init = true;
	filtering = true;
	repeating = false;
	SwitchToPerspective();



}


Renderer::~Renderer() {
	delete meshes[0];
	delete meshes[1];
	delete shader;
	delete camera;
	glDeleteTextures(2, textures);
}


void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}


Matrix4 Renderer::GetModelMatrix(int mesh) {
	return Matrix4::Translation(positions[mesh]) * Matrix4::Rotation(rotations[mesh], { 0, 1, 0 }) * Matrix4::Scale(scales[mesh]);
}


void Renderer::RenderScene() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BindShader(shader);
	UpdateShaderMatrices();
	
	for (int i = 0; i < 2; i++) {
		modelMatrix = GetModelMatrix(i);
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
		
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		meshes[i]->Draw();
	}
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}


void Renderer::ToggleObject() {
	modifyObject = !modifyObject;
}


void Renderer::ToggleBlendMode() {
	blendMode = (blendMode + 1) % 4;
	switch (blendMode) {
	case 0: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
	case 1: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); break;
	case 2: glBlendFunc(GL_ONE, GL_ZERO); break;
	case 3: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	}
}


void Renderer::ToggleDepth() {
	usingDepth = !usingDepth;
	usingDepth ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}


void Renderer::ToggleAlphaBlend() {
	usingAlpha = !usingAlpha;
	usingAlpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}


void Renderer::MoveObject(float by) {
	positions[(int)modifyObject].z += by;
}
