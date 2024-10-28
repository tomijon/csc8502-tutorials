#include "Renderer.h"
#include "shapes.hpp"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = new Triangle;
	camera = new Camera(0, 0, { 0, 0, 0 });

	basicShader = new Shader("tut3_vertex.glsl", "tut3_fragment.glsl");
	if (!basicShader->LoadSuccess()) {
		return;
	}

	texture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!texture) {
		return;
	}

	init = true;
	filtering = true;
	repeating = false;
	SwitchToPerspective();
}


Renderer::~Renderer() {
	delete triangle;
	delete basicShader;
	glDeleteTextures(1, &texture);
}


void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}


void Renderer::RenderScene() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(basicShader);
	modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Rotation(0, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(10, 10, 10));
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(basicShader->GetProgram(), "diffuseText"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	triangle->Draw();
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}


