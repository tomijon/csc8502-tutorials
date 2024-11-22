#pragma once

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Window.h"
#include "Camera.hpp"
#include "SceneGraph.hpp"
#include "shapes.hpp"
#include "Animator.hpp"
#include "Planet.hpp"
#include <string>
#include <unordered_map>
#include <any>


namespace Render {

	// Enums of each post processing to be done. Written in the order they should be
	// executed.
	enum PostProcessFramebuffer {
		BLOOM = 1,
		BW = 2,
		BW_SMOOTH = 3,
		SMOOTH = 4,
		BLUR = 5
	};

	// Class for handling which post processes are active.
	class ActivePostProcesses {
		static int active;

	public:
		static void enable(PostProcessFramebuffer postProcess) {
			active += 1 << postProcess;
		}

		static void disable(PostProcessFramebuffer postProcess) {
			active &= ~(1 << postProcess);
		}

		static bool isActive(PostProcessFramebuffer postProcess) {
			return ((active >> postProcess) & 1) == 1;
		}
	};


	class UniformDataBase {
	public:
		UniformDataBase() {}
		virtual ~UniformDataBase() {};

		virtual void sendData(const Shader* shader) const {};

		void updateData(std::any data) { m_data = data; }
		std::any getData() { return m_data; }

	private:
		std::any m_data;
	};


	class PostProcess {
	public:
		friend class OGLRenderer;
		PostProcess(int width, int height, Shader* shader) : shader(shader), width(width), height(height) {
			generateColorTexture();
			generateFramebuffer();

			if (!framebuffer || !colorOutput) return;
			success = true;
		}

		~PostProcess() {
			glDeleteFramebuffers(1, &framebuffer);
			glDeleteTextures(1, &colorOutput);
			delete shader;
		}

		GLuint getOutputBuffer() { return colorOutput; }
		GLuint getInputBuffer() { return inputBuffer; }
		void setInputBuffer(GLuint input) { inputBuffer = input; }

		void bind() {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
		
		void setActive(bool b) { active = b; }
		bool isActive() { return active; }

		void setPasses(unsigned int p) { passes = p; }
		unsigned int getPasses() { return passes; }

		Shader* getShader() { return shader; }

		bool hasInitialised() { return success; }


	private:
		void generateColorTexture() {
			glGenTextures(1, &colorOutput);
			glBindTexture(GL_TEXTURE_2D, colorOutput);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}

		void generateFramebuffer() {
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				colorOutput, 0);
		}

		Shader* shader;

		int width;
		int height;

		GLuint framebuffer;
		GLuint inputBuffer;
		GLuint colorOutput;

		bool success = true;
		bool active = true;
		unsigned int passes = 1;
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

	void toggleBloom() { bloom->setActive(!bloom->isActive()); }
	void toggleBlackAndWhite() { blackAndWhite->setActive(!blackAndWhite->isActive()); }
	void toggleBlackAndWhiteSmooth() { blackAndWhiteSmooth->setActive(!blackAndWhiteSmooth->isActive()); }
	void toggleSmooth() { smooth->setActive(!smooth->isActive()); }
	void toggleBlur() { blur->setActive(!blur->isActive()); }

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
	Mesh* screen;

	Planet* planet;
	Planet* water;
	Planet* sun;

	Frustum frameFrustum;

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;


	GLuint renderFramebuffer;
	GLuint renderColorOutput;
	GLuint renderDepthOutput;

	Render::PostProcess* bloom;
	Render::PostProcess* blackAndWhite;
	Render::PostProcess* blackAndWhiteSmooth;
	Render::PostProcess* smooth;
	Render::PostProcess* blur;
};

