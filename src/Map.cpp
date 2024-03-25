#include "Map.h"
#include "Renderer.h"

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include <Noise/PerlinNoise.hpp>
#include <yaml/Yaml.hpp>

namespace Ecosim
{
    void SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
    {
        uint8_t *pixelAddr = (uint8_t *)surface->pixels + y * surface->pitch + x * sizeof(uint32_t);
        *(uint32_t *)pixelAddr = pixel;
    }

    uint32_t GetPixel(SDL_Surface *surface, int x, int y)
    {
        uint8_t *pixelAddr = (uint8_t *)surface->pixels + y * surface->pitch + x * sizeof(uint32_t);
        return *(uint32_t *)pixelAddr;
    }

    void GenerateMap(SDL_Surface *surface, float waterHeight = 0.5f, double smoothness = 0.006, siv::PerlinNoise::seed_type seed = 0)
    {
        const siv::PerlinNoise perlin(seed);

        SDL_LockSurface(surface);

        for (int x = 0; x < surface->w; ++x)
        {
            for (int y = 0; y < surface->h; ++y)
            {
                double noise = perlin.octave2D_01(x * smoothness, y * smoothness, 4);

                // uint32_t waterColor = SDL_MapRGB(surface->format, 51, 77, 102);
                // uint32_t landColor = SDL_MapRGB(surface->format, 80, 140, 30);

                // uint32_t color = noise > waterHeight ? landColor : waterColor;
                // uint32_t color = waterColor * noise + landColor * (1 - noise);
                uint32_t color = SDL_MapRGB(surface->format, (uint32_t)(noise * 255.0), (uint32_t)(noise * 255.0), (uint32_t)(noise * 255.0));

                SetPixel(surface, x, y, color);
            }
        }

        SDL_UnlockSurface(surface);
    }

    std::vector<Vector2<int>> DistributeBiomes(uint numBiomes, uint mapWidth, uint mapHeight)
    {
        std::random_device randomDevice;
        std::mt19937 generator(randomDevice());

        std::uniform_int_distribution<> randomWidth(0, mapWidth - 1);
        std::uniform_int_distribution<> randomHeight(0, mapHeight - 1);

        std::vector<Vector2<int>> centers;
        centers.reserve(numBiomes);

        for (uint i = 0; i < numBiomes; ++i)
        {
            int x = randomWidth(generator), y = randomHeight(generator);
            centers.push_back(Vector2<int>(x, y));
        }

        return centers;
    }

    int FindClosestBiome(int x, int y, std::vector<Vector2<int>> &centers)
    {
        float closestDistance = 10000000.0f;
        int closestIndex = 0;

        float fx = static_cast<float>(x);
        float fy = static_cast<float>(y);

        int index = 0;
        for (const auto &center : centers)
        {
            float dx = static_cast<float>(center.x) - fx;
            float dy = static_cast<float>(center.y) - fy;

            float distance = dx * dx + dy * dy;
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestIndex = index;
            }
            ++index;
        }

        return closestIndex;
    }

    void GenerateBiomes(SDL_Surface *surface, std::vector<Vector2<int>> &centers)
    {
        std::vector<uint32_t> colors = {
            SDL_MapRGB(surface->format, 156, 196, 47),
            SDL_MapRGB(surface->format, 102, 124, 39),
            SDL_MapRGB(surface->format, 81, 168, 57),
            SDL_MapRGB(surface->format, 57, 168, 103),
            SDL_MapRGB(surface->format, 32, 178, 93),
            SDL_MapRGB(surface->format, 72, 160, 64),
        };

        SDL_LockSurface(surface);

        for (int x = 0; x < surface->w; ++x)
        {
            for (int y = 0; y < surface->h; ++y)
            {
                uint32_t color = colors[FindClosestBiome(x, y, centers) % colors.size()];
                SetPixel(surface, x, y, color);
            }
        }

        SDL_UnlockSurface(surface);
    }

    void ApplyBiomeSmoothing(SDL_Surface *surface)
    {
        int kernelSize = 5; // 3 / 2 = 1
        int halfKernelSize = kernelSize / 2;

        std::vector<uint32_t> newColors;
        newColors.reserve(surface->w * surface->h);

        SDL_LockSurface(surface);

        for (int i = 0; i < halfKernelSize; ++i)
        {
            for (int x = 0; x < surface->w; ++x)
                newColors.push_back(GetPixel(surface, x, i));
        }

        for (int y = halfKernelSize; y < surface->h - halfKernelSize - 1; ++y)
        {
            for (int i = 0; i < halfKernelSize; ++i)
            {
                newColors.push_back(GetPixel(surface, i, y));
            }

            for (int x = halfKernelSize; x < surface->w - halfKernelSize - 1; ++x)
            {
                std::unordered_map<uint32_t, int> countMap;

                for (int i = -halfKernelSize; i <= halfKernelSize; ++i)
                {
                    for (int j = -halfKernelSize; j <= halfKernelSize; ++j)
                        ++countMap[GetPixel(surface, x + i, y + j)];
                }

                uint32_t mostCommon = 0;
                int maxCount = 0;
                for (const auto &pair : countMap)
                {
                    if (pair.second > maxCount)
                    {
                        mostCommon = pair.first;
                        maxCount = pair.second;
                    }
                }

                newColors.push_back(mostCommon);
            }

            for (int i = 0; i < halfKernelSize + 1; ++i)
            {
                newColors.push_back(GetPixel(surface, surface->w - (halfKernelSize + 1 - i), y));
            }
        }

        for (int i = 0; i < halfKernelSize + 1; ++i)
        {
            for (int x = 0; x < surface->w; ++x)
                newColors.push_back(GetPixel(surface, x, surface->h - (halfKernelSize + 1 - i)));
        }

        SDL_memcpy(surface->pixels, newColors.data(), surface->h * surface->pitch);

        SDL_UnlockSurface(surface);
    }

    void MergeSurfaces(SDL_Surface *finalSurface, SDL_Surface *heightmapSurface, SDL_Surface *biomeSurface)
    {
        SDL_LockSurface(finalSurface);
        SDL_LockSurface(heightmapSurface);
        SDL_LockSurface(biomeSurface);

        for (int x = 0; x < finalSurface->w; ++x)
        {
            for (int y = 0; y < finalSurface->h; ++y)
            {
                uint32_t mask = (GetPixel(heightmapSurface, x, y) & 255) > 160u ? 1u : 0u;
                uint32_t color = GetPixel(biomeSurface, x, y) * mask + SDL_MapRGB(finalSurface->format, 51, 77, 102) * (1u - mask);

                SetPixel(finalSurface, x, y, color);
            }
        }

        SDL_UnlockSurface(finalSurface);
        SDL_UnlockSurface(heightmapSurface);
        SDL_UnlockSurface(biomeSurface);
    }

    Map::Map()
    {
        Yaml::Node root;
        Yaml::Parse(root, "configs/enviroments/islands.yaml");

        Yaml::Node &generationNode = root["generation"];
        // if (generationNode.IsNone())

        uint32_t seed = generationNode["seed"].As<uint32_t>();
        double smoothness = generationNode["smoothness"].As<double>();
        float waterLevel = generationNode["water-level"].As<float>();
        uint mapWidth = generationNode["map-width"].As<uint>(100);
        uint mapHeight = generationNode["map-height"].As<uint>(100);

        SDL_Surface *heightmapSurface = Renderer::RequestSDLSurface(mapWidth, mapHeight);

        GenerateMap(heightmapSurface, waterLevel, smoothness, seed);

        SDL_Surface *biomeSurface = Renderer::RequestSDLSurface(mapWidth, mapHeight);

        std::vector<Vector2<int>> centers = DistributeBiomes(20, mapWidth, mapHeight);
        GenerateBiomes(biomeSurface, centers);

        // for (int i = 0; i < 1; ++i)
        ApplyBiomeSmoothing(biomeSurface);

        SDL_Surface *finalSurface = Renderer::RequestSDLSurface(mapWidth, mapHeight);

        MergeSurfaces(finalSurface, heightmapSurface, biomeSurface);

        m_texture = Renderer::BakeTexture(finalSurface);

        SDL_FreeSurface(biomeSurface);
        SDL_FreeSurface(heightmapSurface);
        SDL_FreeSurface(finalSurface);
    }

    void Map::Render()
    {
        Renderer::Texture(m_texture, Vector2<int>(0, 0));
    }

    Map::~Map()
    {
        SDL_DestroyTexture(m_texture);
    }
}