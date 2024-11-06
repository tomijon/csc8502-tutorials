#include "Renderer.h"
#include "shapes.hpp"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	meshes[0] = new Triangle;
	meshes[1] = new Quad;
	camera = new Camera(0, 0, { 0, 0, 0 });

	shader = new Shader("tut5_vertex.glsl", "tut5_fragment.glsl");
	if (!shader->LoadSuccess()) return;

	textures[0] = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!textures[0]) return;
	SetTextureRepeating(textures[0], true);
	
	textures[1] = SOIL_load_OGL_texture(TEXTUREDIR"star.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	if (!textures[1]) return;
	SetTextureRepeating(textures[1], true);

	positions[0] = { 0, 0, 0 };
	positions[1] = { 0, 0, 0 };

	scales[0] = { 10, 10, 10 };
	scales[1] = { 10, 10, 10 };

	rotations[0] = 0;
	rotations[1] = 0;

	usingScissor = false;
	usingStencil = false;
	SwitchToPerspective();
	init = true;
}


Renderer::~Renderer() {
	delete meshes[0];
	delete meshes[1];
	delete shader;
	delete camera;
	glDeleteTextures(2, textures);
}


void Renderer::ToggleScissor() {
	usingScissor = !usingScissor;
}


void Renderer::ToggleStencil() {
	usingStencil = !usingStencil;
}


Matrix4 Renderer::GetModelMatrix(int mesh) {
	return Matrix4::Translation(positions[mesh]) * Matrix4::Rotation(rotations[mesh], { 0, 1, 0 }) * Matrix4::Scale(scales[mesh]);
}


void Renderer::RenderScene() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	if (usingScissor) {
		glEnable(GL_SCISSOR_TEST);
		glScissor((float)width / 2.5f, (float)height / 2.5f,
			(float)width / 5.0f, (float)height / 5.0f);
	}




	BindShader(shader);
	textureMatrix = Matrix4::Scale(Vector3(1, 1, 1));
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	UpdateShaderMatrices();


	if (usingStencil) {
		glEnable(GL_STENCIL_TEST);
	
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_ALWAYS, 2, ~0);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		modelMatrix = GetModelMatrix(1);
		glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
		meshes[1]->Draw();
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, 2, ~0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		
	}

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	modelMatrix = GetModelMatrix(0);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
	meshes[0]->Draw();
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
}


void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}


void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1, 10000, (float)width / (float)height, 45);
}
