#pragma once

#include <string>
#include <unordered_map>

class Vector2;
class Mesh;

namespace Terrain {
	class NoiseGenerator;
	class HydraulicEroder;
	class Heightmap;
	class Droplet;
	

	struct Point {
		unsigned int x;
		unsigned int y;

		bool operator==(const Point& other) const {
			return other.x == x and other.y == y;
		}
	};


	struct PointHash {
		size_t operator()(const Point& point) const {
			return (static_cast<size_t>(point.x) << 32) ^ point.y;
		}
	};
	
	
	// Perlin Noise Generator.
	class NoiseGenerator {
	public:
		NoiseGenerator(Vector2 dimensions);
		~NoiseGenerator();
		
		void create();
		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }
		inline float* getNoise() const { return image; }

		inline void setSeed(int seed) { randomSeed = seed; }
		inline void setLayers(int l) { layers = l; }
		inline void setFreq(float freq) { this->freq = freq; }
		inline void setFreqMult(float mult) { freqMult = mult; }
		inline void setAmp(float amp) { this->amp = amp; }
		inline void setAmpMult(float mult) { ampMult = mult; }
		
		int pointToIndex(int x, int y) const;
		Vector2 getGradient(int x, int y);
		void resetGradients();

		float fade(float val) const;

	protected:
		unsigned int width;
		unsigned int height;
		int randomSeed;
		unsigned int layers;
		
		float freq;
		float amp;
		float freqMult;
		float ampMult;

		void generate();
		float calculateHeight(float x, float y);
		void normalise(float lowest, float highest);

		void applyDistance();
		float getDistance(int x, int y);
		Vector2 generateGradient(int x, int y);
		float lerp(float low, float high, float weight) const;


	private:
		float* image;
		std::unordered_map<Point, Vector2, PointHash> gradients;
	};


	class HydraulicEroder {
	public:
	private:
	};


	class Heightmap : public Mesh {
	public:
		Heightmap(const NoiseGenerator& noise);
		~Heightmap(void) {};

		Vector3 GetHeightmapSize() const { return heightmapSize; }

	private:
		Vector3 heightmapSize;
	};


	class Droplet {
	public:
	protected:
	};


	float dot(const Vector2&, const Vector2&);
	Vector2 normalise(const Vector2&);
}