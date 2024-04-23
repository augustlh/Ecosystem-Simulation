#include "Generation.h"
#include "Vector2.h"
#include <PerlinNoise/PerlinNoise.hpp>
#include <random>

namespace Ecosim::Generation
{
    uint seed = 0;
    void Seed(uint _seed) { seed = _seed; }

    std::vector<Vector2<uint>> VoronoiCenters(uint width, uint height, uint n)
    {
        std::vector<Vector2<uint>> centers;
        centers.reserve(n);

        std::mt19937 gen(seed);
        std::uniform_int_distribution<> newWidth(0, width);
        std::uniform_int_distribution<> newHeight(0, height);

        for (int i = 0; i < n; ++i)
        {
            Vector2<uint> center(newWidth(gen), newHeight(gen));
            centers.emplace_back(center);
        }

        return centers;
    }

    uint ClosestCenter(uint x, uint y, std::vector<Vector2<uint>> &centers)
    {
        uint minDistance = ~0u;
        uint minIndex = 0;

        uint dx, dy, dist;

        for (uint i = 0; i < centers.size(); ++i)
        {
            dx = x - centers[i].x;
            dy = y - centers[i].y;
            dist = dx * dx + dy * dy;

            if (dist < minDistance)
            {
                minDistance = dist;
                minIndex = i;
            }
        }

        return minIndex;
    }

    std::vector<uint> Voronoi(uint width, uint height, uint numPoints)
    {
        std::vector<Vector2<uint>> centers = std::move(VoronoiCenters(width, height, numPoints));

        std::vector<uint> texture;
        texture.reserve(width * height);

        for (uint x = 0; x < width; ++x)
        {
            for (uint y = 0; y < height; ++y)
                texture.emplace_back(ClosestCenter(x, y, centers));
        }

        return texture;
    }

    std::vector<uint> DetailedVoronoi(uint width, uint height, uint numPoints, uint detailLayers)
    {
        std::vector<uint> texture = std::move(Voronoi(width, height, numPoints));

        for (uint i = 0; i < detailLayers; ++i)
        {
            uint n = static_cast<uint>(numPoints * 1.6f * (i + 1));
            std::vector<Vector2<uint>> centers = VoronoiCenters(width, height, n);

            std::vector<uint> identifier;
            identifier.reserve(centers.size());

            for (auto &center : centers)
                identifier.emplace_back(texture[INDEX_FROM_COORD(center.x, center.y, height)]);

            for (uint x = 0; x < width; ++x)
            {
                for (uint y = 0; y < height; ++y)
                    texture[INDEX_FROM_COORD(x, y, height)] = identifier[ClosestCenter(x, y, centers)];
            }
        }

        return texture;
    }

    std::vector<float> Perlin(uint width, uint height, float smoothness, int octaves)
    {
        siv::PerlinNoise perlin(seed);

        std::vector<float> noises;
        noises.reserve(width * height);

        for (uint x = 0; x < width; ++x)
        {
            for (uint y = 0; y < height; ++y)
            {
                float noise = perlin.octave2D_01(x * smoothness, y * smoothness, octaves);
                noises.push_back(noise);
            }
        }

        return noises;
    }

    void MapVoronoi(std::vector<uint> &texture, uint numPoints, const std::vector<float> probabilities)
    {
        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> range01(0.0f, 1.0f);

        std::vector<uint> mapper(numPoints, 0);

        uint to = 0;
        for (auto &from : mapper)
        {
            while (true)
            {
                float r = range01(gen);
                if (r <= probabilities[to])
                    break;
                to = (to + 1) % probabilities.size();
            }
            from = to;
        }

        for (auto &pixel : texture)
            pixel = mapper[pixel];
    }

} // namespace Ecosim::Generation
