#ifndef GENERATION_H
#define GENERATION_H

#include <vector>

#define INDEX_FROM_COORD(x, y, h) static_cast<uint>(x * h + y)

typedef unsigned int uint;

namespace Ecosim::Generation
{
    void Seed(uint seed);
    std::vector<uint> Voronoi(uint width, uint height, uint numPoints);

    // std::vector<uint> DetailedVoronoi(uint width, uint height, uint numPoints, uint detailLayers = 1);
    std::vector<uint> DetailedVoronoi(uint width, uint height, uint numPoints, uint detailLayers);
    inline std::vector<uint> DetailedVoronoi(uint width, uint height, uint numPoints) { return DetailedVoronoi(width, height, numPoints, 1); }

    // std::vector<float> Perlin(uint width, uint height, float smoothness = 0.005f, int octaves = 4);
    std::vector<float> Perlin(uint width, uint height, float smoothness, int octaves);
    inline std::vector<float> Perlin(uint width, uint height, float smoothness) { return Perlin(width, height, smoothness, 4); };
    inline std::vector<float> Perlin(uint width, uint height) { return Perlin(width, height, 0.005f, 4); };

    void MapVoronoi(std::vector<uint> &texture, uint numPoints, const std::vector<float> probabilities);

} // namespace Ecosim::Generation

#endif /* GENERATION_H */