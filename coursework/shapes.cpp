#include "shapes.hpp"

Triangle::Triangle() {
	numVertices = 3;

	vertices = new Vector3[numVertices]{
		{0.5, 1, 0},
		{1, 0, 0},
		{0, 0, 0}
	};

	colours = new Vector4[numVertices]{
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1}
	};

	textureCoords = new Vector2[numVertices];
	textureCoords[0] = Vector2(0.5, 0);
	textureCoords[1] = Vector2(1, 1);
	textureCoords[2] = Vector2(0, 1);

	BufferData();
}


Quad::Quad() {
	numVertices = 4;
	numIndices = 4;

	vertices = new Vector3[numVertices]{
		{-1, 1, 0},
		{-1, -1, 0},
		{1, 1, 0},
		{1, -1, 0}
	};

	indices = new unsigned int[numIndices] {
		0, 1, 2, 3
	};


	textureCoords = new Vector2[numVertices];
	textureCoords[0] = { 0, 1 };
	textureCoords[1] = { 0, 0 };
	textureCoords[2] = { 1, 1 };
	textureCoords[3] = { 1, 0 };

	colours = new Vector4[numVertices];
	normals = new Vector3[numVertices]; 
	tangents = new Vector4[numVertices];

	for (int i = 0; i < 4; ++i) {
	    colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		normals[i] = Vector3(0.0f, 0.0f, -1.0f); 
		tangents[i] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	type = GL_TRIANGLE_STRIP;
	BufferData();
}




CubeRobot::CubeRobot(Mesh * cube) {
	// SetMesh ( cube ); // Uncomment if you want a local origin marker !
	SceneNode * body = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	body->SetModelScale(Vector3(10, 15, 5));
	body->SetTransform(Matrix4::Translation(Vector3(0, 35, 0)));
	AddChild(body);
	
	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head->SetModelScale(Vector3(5, 5, 5));
	head->SetTransform(Matrix4::Translation(Vector3(0, 30, 0)));
	body->AddChild(head);

	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm->SetModelScale(Vector3(3, -18, 3));
	leftArm->SetTransform(Matrix4::Translation(Vector3(-12, 30, -1)));
	body->AddChild(leftArm);

	rightArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightArm->SetModelScale(Vector3(3, -18, 3));
	rightArm->SetTransform(Matrix4::Translation(Vector3(12, 30, -1)));
	body->AddChild(rightArm);

	SceneNode* leftLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftLeg->SetModelScale(Vector3(3, -17.5, 3));
	leftLeg->SetTransform(Matrix4::Translation(Vector3(-8, 0, 0)));
	body->AddChild(leftLeg);

	SceneNode* rightLeg = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	rightLeg->SetModelScale(Vector3(3, -17.5, 3));
	rightLeg->SetTransform(Matrix4::Translation(Vector3(8, 0, 0)));
	body->AddChild(rightLeg);


	body->SetBoundingRadius(15.0f);
	head->SetBoundingRadius(5.0f);

	leftArm->SetBoundingRadius(18.0f);
	rightArm->SetBoundingRadius(18.0f);

	leftLeg->SetBoundingRadius(18.0f);
	rightLeg->SetBoundingRadius(18.0f);
}


void CubeRobot::Update(float dt) {
	transform = transform * Matrix4::Rotation(30.0f * dt, Vector3(0, 1, 0));
	head->SetTransform(head->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0)));
	leftArm->SetTransform(leftArm->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(1, 0, 0)));
	rightArm->SetTransform(rightArm->GetTransform() * Matrix4::Rotation(30.0f * dt, Vector3(1, 0, 0)));
	SceneNode::Update(dt);
}


Plane::Plane(const Vector3& normal, float distance, bool normalise) {
	if (normalise) {
		float length = sqrt(Vector3::Dot(normal, normal));

		this->normal = normal / length;
		this->distance = distance / length;
	}
	else {
		this->normal = normal;
		this->distance = distance;
	}
}


bool Plane::SphereInPlane(const Vector3& position, float radius) const {
	return !(Vector3::Dot(position, normal) + distance <= -radius);
}


bool Frustum::InsideFrustum(SceneNode& node) {
	for (int p = 0; p < 6; p++) {
		if (!planes[p].SphereInPlane(node.GetWorldTransform().GetPositionVector(), node.GetBoundingRadius())) return false;
	}
	return true;
}


void Frustum::FromMatrix(const Matrix4& mat) {
	Vector3 xaxis = Vector3(mat.values[0], mat.values[4], mat.values[8]);
	Vector3 yaxis = Vector3(mat.values[1], mat.values[5], mat.values[9]);
	Vector3 zaxis = Vector3(mat.values[2], mat.values[6], mat.values[10]);
	Vector3 waxis = Vector3(mat.values[3], mat.values[7], mat.values[11]);

	planes[0] = Plane(waxis - xaxis, (mat.values[15] - mat.values[12]), true);
	planes[1] = Plane(waxis + xaxis, (mat.values[15] + mat.values[12]), true);
	planes[2] = Plane(waxis + yaxis, (mat.values[15] + mat.values[13]), true);
	planes[3] = Plane(waxis - yaxis, (mat.values[15] - mat.values[13]), true);
	planes[4] = Plane(waxis + zaxis, (mat.values[15] + mat.values[14]), true);
	planes[5] = Plane(waxis - zaxis, (mat.values[15] - mat.values[14]), true);
}