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
		{1, 0, 0, 1},
		{1, 0, 0, 1}
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
		{0, 0, 0},
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0}
	};

	indices = new unsigned int[numIndices] {
		0, 1, 2, 3
	};

	colours = new Vector4[numVertices];
	for (int i = 0; i < numVertices; i++) {
		colours[i] = { 1, 1, 1, 1 };
	}

	textureCoords = new Vector2[numVertices];
	textureCoords[0] = { 0, 0 };
	textureCoords[1] = { 1, 0 };
	textureCoords[2] = { 0, 1 };
	textureCoords[3] = { 1, 1 };

	type = GL_TRIANGLE_STRIP;
	BufferData();
}