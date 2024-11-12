#include "heightmap.hpp"
#include <iostream>

HeightMap::HeightMap(const std::string& name) {
	int iWidth, iHeight, iChans;

	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);
	if (!data) {
		std::cout << "Could not load heightmap file.\n";
		return;
	}

	numVertices = iWidth * iHeight;
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];
	float max = 0;

	Vector3 vertexScale = Vector3(16, 1, 16);
	Vector2 textureScale = Vector2(1.0f / 16.0f, 1.0f / 16.0f);

	for (int z = 0; z < iHeight; z++) {
		for (int x = 0; x < iWidth; x++) {
			int offset = (z * iWidth) + x;
			vertices[offset] = Vector3(x, 8 * data[offset], z) * vertexScale;

			if (vertices[offset].y > max) max = vertices[offset].y;

			textureCoords[offset] = Vector2(x, z) * textureScale;
		}
	}
	SOIL_free_image_data(data);

	int i = 0;
	for (int z = 0; z < iHeight - 1; ++z) {
		for (int x = 0; x < iWidth - 1; ++x) {
			int a = (z * (iWidth)) + x;
			int b = (z * (iWidth)) + (x + 1);
			int c = ((z + 1) * (iWidth)) + (x + 1);
			int d = ((z + 1) * (iWidth)) + x;
			
			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;
			
			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}
	GenerateNormals();
	BufferData();

	heightmapSize.x = vertexScale.x * (iWidth - 1);
	heightmapSize.y = vertexScale.y * 255.0f; // each height is a byte !
	heightmapSize.z = vertexScale.z * (iHeight - 1);
}