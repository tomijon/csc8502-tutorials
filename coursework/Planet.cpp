#include "Planet.hpp"

PlanetSurface::PlanetSurface(Vector3 up, int size) : up(up), size(size) {
	axis0 = Vector3(up.y, up.z, up.x);
	axis1 = Vector3::Cross(up, axis0);
}


void PlanetSurface::generateMesh() {
	numVertices = size * size;
	numIndices = std::pow(size - 1, 2) * 6;
	vertices = new Vector3[numVertices];
	indices = new unsigned int[numIndices];
	textureCoords = new Vector2[numVertices];

	Vector2 textureScale = Vector2(1.0f / 16.0f, 1.0f / 16.0f);

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			float xPercent = x / static_cast<float>(size - 1);
			float zPercent = z / static_cast<float>(size - 1);
			Vector3 vert;
			vert += up + (axis0 * (xPercent - 0.5) * 2);
			vert += up + (axis1 * (zPercent - 0.5) * 2);
			vert.Normalise();
			
			vertices[(z * size) + x] = vert;
			textureCoords[(z * size) + x] = Vector2(x, z) * textureScale;

		}
	}

	int tri = 0;
	for (int z = 0; z < size - 1; z++) {
		for (int x = 0; x < size - 1; x++) {
			int a = (z * size) + x;
			int b = (z * size) + (x + 1);
			int c = ((z + 1) * size) + (x + 1);
			int d = ((z + 1) * size) + x;

			indices[tri++] = c;
			indices[tri++] = a;
			indices[tri++] = b;

			indices[tri++] = a;
			indices[tri++] = c;
			indices[tri++] = d;
		}
	}
}


void PlanetSurface::applyNoise(std::string name) {
	int iWidth, iHeight, iChans;

	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);
	if (!data) {
		std::cout << "Could not load heightmap file.\n";
		return;
	}

	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {
			int offset = (z * size) + x;
			vertices[offset] = (vertices[offset] * 16) + (vertices[offset]) * data[offset] / 64;
		}
	}

	//for (int vert = 0; vert < numVertices; vert++) {
	//	vertices[vert] = vertices[vert] * data[vert] * 255;
	//}
	SOIL_free_image_data(data);
	GenerateNormals();
	BufferData();
}