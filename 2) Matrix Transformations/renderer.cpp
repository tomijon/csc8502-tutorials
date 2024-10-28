#include "Renderer.h"
#include "shapes.hpp"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = new Triangle;
	camera = new Camera(0, 0, { 0, 0, 0 });
	basicShader = new Shader("tut2_vertex.glsl", "tut2_fragment.glsl");

	if (!basicShader->LoadSuccess()) {
		return;
	}
	init = true;

	SwitchToPerspective();
}


Renderer::~Renderer() {
	delete triangle;
	delete basicShader;
}


void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}


void Renderer::SetFOV(float fov) {
	this->fov = fov;
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, this->fov);
}


void Renderer::RenderScene() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	BindShader(basicShader);

	modelMatrix = Matrix4::Translation(Vector3(0, 0, 0)) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(10, 10, 10));
	UpdateShaderMatrices();
	glUniformMatrix4fv(glGetUniformLocation(basicShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
	triangle->Draw();
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}