#include "SceneGraph.hpp"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) : mesh(mesh), colour(colour) {
	parent = nullptr;
	modelScale = Vector3(1, 1, 1);

	boundingRadius = 1;
	distanceFromCamera = 0;
	drawDistance = 0;
}


SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); i++) {
		delete children[i];
	}
}


void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}


void SceneNode::Draw(const OGLRenderer& renderer) {
	if (mesh) mesh->Draw();
}


void SceneNode::Update(float dt) {
	if (parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else {
		worldTransform = transform;
	}

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); i++) {
		(*i)->Update(dt);
	}
}