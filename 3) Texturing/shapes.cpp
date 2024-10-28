#include "shapes.hpp"

Triangle::Triangle() {
	numVertices = 3;

	vertices = new Vector3[numVertices]{
		{0, 0.5, 0},
		{0.3, -0.5, 0},
		{-0.3, -0.5, 0}
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