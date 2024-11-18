#pragma once

#include "../nclgl/Matrix4.h"
#include "../nclgl/Vector3.h"
#include "../nclgl/Vector4.h"
#include "../nclgl/Mesh.h"
#include <vector>


class SceneNode {
public:
	SceneNode(Mesh* m = nullptr, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	Shader* GetShader() const { return shader; }
	void SetShader(Shader* s) { shader = s; }
	
	void AddChild(SceneNode* s);

	virtual void Update(float dt);
	virtual void Draw(const OGLRenderer& r);

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetDrawDistance() const { return drawDistance; }
	void SetDrawDistance(float f) { drawDistance = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	virtual void AddTexture(std::string uniformName, GLuint texture, GLuint bump = 0) {
		names.push_back(uniformName);
		textures.push_back(texture);
		bumps.push_back(bump);
	}

	std::vector<GLuint>::const_iterator GetTextureIteratorStart() { return textures.begin(); }
	std::vector<GLuint>::const_iterator GetTextureIteratorEnd() { return textures.end(); }
	std::vector<GLuint>::const_iterator GetBumpIteratorStart() { return bumps.begin(); }
	std::vector<GLuint>::const_iterator GetBumpIteratorEnd() { return bumps.end(); }
	std::vector<std::string>::const_iterator GetNameIteratorStart() { return names.begin(); }
	std::vector<std::string>::const_iterator GetNameIteratorEnd() { return names.end(); }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera);
	}

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

protected:
	SceneNode* parent;
	Mesh* mesh;
	Shader* shader;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;
	std::vector<SceneNode*> children;

	float distanceFromCamera;
	float drawDistance;
	float boundingRadius;
	std::vector<std::string> names;
	std::vector<GLuint> textures;
	std::vector<GLuint> bumps;
};
