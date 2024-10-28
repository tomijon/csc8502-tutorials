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
		{0, 1, 0, 1},
		{0, 0, 1, 1}
	};

	BufferData();
}