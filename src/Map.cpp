#include "Map.h"
#include "Renderer.h"

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include <PerlinNoise/PerlinNoise.hpp>
#include <MiniYaml/Yaml.hpp>

namespace Ecosim
{
    std::vector<Biome> GenerateBiomeTypes(Yaml::Node &biomesNode)
    {
        std::vector<Biome> biomes;

        for (uint i = 0; i < biomesNode.Size(); ++i)
        {
            Yaml::Node &item = biomesNode[i];

            std::string name = item["name"].As<std::string>("<unnamed biome>");
            float probability = item["probability"].As<float>(0.0f);
            Color color = Color(item["color"][0].As<int>(0), item["color"][1].As<int>(0), item["color"][2].As<int>(0));

            biomes.push_back(Biome(name, probability, color));
        }

        return biomes;
    }

    void SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
    {
        uint32_t *pixelAddr = (uint32_t *)surface->pixels + y * surface->w + x;
        *pixelAddr = pixel;
    }

    size_t IndexFromCoordinate(uint x, uint y, uint h)
    {
        return x * h + y;
    }

    std::vector<float> GenerateHeightMap(uint w, uint h, float smoothness = 0.005f, uint seed = 0)
    {
        siv::PerlinNoise perlin(seed);

        std::vector<float> heightMap;
        heightMap.reserve(w * h);

        for (uint x = 0; x < w; ++x)
        {
            for (uint y = 0; y < h; ++y)
            {
                float noise = perlin.octave2D_01(x * smoothness, y * smoothness, 4);
                heightMap.push_back(noise);
            }
        }

        return heightMap;
    }

    struct BiomeCenter
    {
        Vector2<int> center;
        uint type;
        BiomeCenter(Vector2<int> _center, uint _type) : center(_center), type(_type) {}
    };

    std::vector<BiomeCenter> DistributeBiomesVoronoi(uint mapWidth, uint mapHeight, uint numPoints, std::vector<Biome> &biomes, uint seed)
    {
        std::mt19937 generator(seed);

        std::uniform_int_distribution<> randomWidth(0, mapWidth - 1);
        std::uniform_int_distribution<> randomHeight(0, mapHeight - 1);
        std::uniform_real_distribution<float> random01(0.0f, 1.0f);

        std::vector<BiomeCenter> biomeCenters;
        biomeCenters.reserve(numPoints);

        uint biomeIndex = 0;
        float nextProbability = 0.0f;

        for (uint i = 0; i < numPoints; ++i)
        {
            do
            {
                ++biomeIndex %= biomes.size();
                nextProbability = biomes[biomeIndex].probability;
            } while (nextProbability < random01(generator));

            int x = randomWidth(generator), y = randomHeight(generator);
            biomeCenters.push_back(BiomeCenter(Vector2<int>(x, y), biomeIndex));
        }

        return biomeCenters;
    }

    uint FindClosestBiomeType(int x, int y, std::vector<BiomeCenter> &biomeCenters)
    {
        float closestDistance = 10000000.0f;
        uint closestType = 0;

        float fx = static_cast<float>(x);
        float fy = static_cast<float>(y);

        for (const auto &biomeCenter : biomeCenters)
        {
            float dx = static_cast<float>(biomeCenter.center.x) - fx;
            float dy = static_cast<float>(biomeCenter.center.y) - fy;

            float distance = dx * dx + dy * dy;
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestType = biomeCenter.type;
            }
        }

        return closestType;
    }

    std::vector<uint> GenerateBiomeMap(uint w, uint h, std::vector<Biome> &biomes, uint numBiomes = 20, uint seed = 0)
    {
        std::vector<uint> biomeMap;
        biomeMap.reserve(w * h);

        std::vector<BiomeCenter> biomeCenters = std::move(DistributeBiomesVoronoi(w, h, numBiomes, biomes, seed));

        siv::PerlinNoise perlin(seed);
        double noiseScale = 25.0f;
        double smoothness = 0.042f;

        for (uint x = 0; x < w; ++x)
        {
            for (uint y = 0; y < h; ++y)
            {
                double fx = static_cast<double>(x);
                double fy = static_cast<double>(y);

                int dx = x + static_cast<int>(perlin.noise2D(fx * smoothness, fy * smoothness) * noiseScale);
                int dy = y + static_cast<int>(perlin.noise2D(fy * smoothness, fx * smoothness) * noiseScale);

                uint type = FindClosestBiomeType(dx, dy, biomeCenters);
                biomeMap.push_back(type);
            }
        }

        return biomeMap;
    }

    SDL_Surface *CreateMapTexture(uint w, uint h, std::vector<float> &heightMap, float waterLevel, std::vector<uint> &biomeMap, std::vector<Biome> &biomes)
    {
        SDL_Surface *surface = Renderer::RequestSurface(w, h);

        for (uint x = 0; x < w; ++x)
        {
            for (uint y = 0; y < h; ++y)
            {
                uint32_t waterColor = SDL_MapRGB(surface->format, 51, 77, 102);
                uint32_t biomeColor = biomes[biomeMap[IndexFromCoordinate(x, y, h)]].color.Format(surface->format);

                uint32_t color = heightMap[IndexFromCoordinate(x, y, h)] > waterLevel ? biomeColor : waterColor;
                SetPixel(surface, x, y, color);
            }
        }

        return surface;
    }

    // =======================
    //           Map
    // =======================

    uint Map::m_width = 0;
    uint Map::m_height = 0;
    std::vector<uint> Map::m_biomeMap = {};
    std::vector<float> Map::m_heightMap = {};
    std::vector<Biome> Map::m_biomes = {};
    SDL_Surface *Map::m_surface = nullptr;
    float Map::m_waterLevel = 0.0f;

    void Map::Create(const char *configPath)
    {
        Yaml::Node root;
        Yaml::Parse(root, configPath);

        // Biome type creation
        Map::m_biomes = GenerateBiomeTypes(root["biomes"]);

        // Terrain and biome generation
        Yaml::Node &generation = root["generation"];

        uint32_t seed = generation["seed"].As<uint32_t>(0);
        if (seed == 0)
        {
            std::random_device rd;
            seed = rd();
        }

        float smoothness = generation["smoothness"].As<float>(0.005f);
        m_width = generation["map-width"].As<uint>(100);
        m_height = generation["map-height"].As<uint>(100);
        Map::m_waterLevel = generation["water-level"].As<float>(0.6f);
        uint numBiomes = generation["num-biomes"].As<uint>(25);

        Map::m_heightMap = std::move(GenerateHeightMap(Map::m_width, m_height, smoothness, seed));
        Map::m_biomeMap = std::move(GenerateBiomeMap(Map::m_width, m_height, Map::m_biomes, numBiomes, seed));

        // Generate map texture
        Map::m_surface = CreateMapTexture(Map::m_width, m_height, Map::m_heightMap, Map::m_waterLevel, Map::m_biomeMap, Map::m_biomes);
    }

    void Map::Cleanup()
    {
        SDL_DestroySurface(m_surface);
        // SDL_FreeSurface(m_surface);
    }

    void Map::Render()
    {
        // Renderer::Surface(Vector2<int>(0, 0), m_surface);
        Renderer::Surface(0, 0, m_surface);
    }

    Biome &Map::BiomeAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, Map::m_height);
        if (index < Map::m_heightMap.size())
            return Map::m_biomes[Map::m_biomeMap[index]];
        return Map::m_biomes[0];
    }

    float Map::HeightAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, Map::m_height);
        if (index < Map::m_heightMap.size())
            return Map::m_heightMap[index];
        return 0.5f;
    }

    bool Map::WaterAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, Map::m_height);
        if (index < Map::m_heightMap.size())
            return Map::m_heightMap[index] > Map::m_waterLevel;
        return false;
    }
}