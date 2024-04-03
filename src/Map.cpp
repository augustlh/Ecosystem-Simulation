#include "Map.h"
#include "Renderer.h"
#include "Exceptions.h"

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

    // ====================================
    //           EnviromentConfig
    // ====================================

    EnviromentConfig::EnviromentConfig(std::string &configPath) { EnviromentConfig(configPath.c_str()); }
    EnviromentConfig::EnviromentConfig(const char *configPath)
    {
        Yaml::Node root;
        try
        {
            Yaml::Parse(root, configPath);
        }
        ECOSIM_CATCH_AND_CALL(Yaml::ParsingException & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to parse yaml source: '%s'\n%s", configPath, e.what()))
        ECOSIM_CATCH_AND_CALL(Yaml::OperationException & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Invalid filepath to yaml source: '%s'", configPath))
        ECOSIM_CATCH_AND_CALL(std::exception & e, SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Failed to open and read yaml source: '%s'\n%s", configPath, e.what()))

        Yaml::Node &generation = root["generation"];

        seed = generation["seed"].As<uint>(0);
        smoothness = generation["smoothness"].As<float>(0.005f);
        waterLevel = generation["water-level"].As<float>(0.6f);
        mapWidth = generation["map-width"].As<uint>(100);
        mapHeight = generation["map-height"].As<uint>(100);
        numBiomes = generation["num-biomes"].As<uint>(25);

        biomes = GenerateBiomeTypes(root["biomes"]);

        if (seed == 0)
        {
            std::random_device rd;
            seed = rd();
        }
    }

    // =======================
    //           Map
    // =======================

    EnviromentConfig Map::m_config;
    std::vector<uint> Map::m_biomeMap = {};
    std::vector<float> Map::m_heightMap = {};
    SDL_Surface *Map::m_surface = nullptr;

    void Map::Create(std::string &configPath) { Create(configPath.c_str()); }
    void Map::Create(const char *configPath)
    {
        m_config = EnviromentConfig(configPath);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Generating map with parameters:\n  - Seed:             %u\n  - Width and height: %ux%u\n  - Smoothness:       %f\n  - Water level:      %f\n  - Number of biomes: %u",
                    m_config.seed, m_config.mapWidth, m_config.mapHeight, m_config.smoothness, m_config.waterLevel, m_config.numBiomes);

        m_heightMap = std::move(GenerateHeightMap(m_config.mapWidth, m_config.mapHeight, m_config.smoothness, m_config.seed));
        m_biomeMap = std::move(GenerateBiomeMap(m_config.mapWidth, m_config.mapHeight, m_config.biomes, m_config.numBiomes, m_config.seed));
        m_surface = CreateMapTexture(m_config.mapWidth, m_config.mapHeight, m_heightMap, m_config.waterLevel, m_biomeMap, m_config.biomes);
    }

    void Map::Cleanup()
    {
        SDL_DestroySurface(m_surface);
    }

    void Map::Render()
    {
        Renderer::Surface(0, 0, m_surface);
    }

    uint Map::Width() { return m_config.mapWidth; }

    uint Map::Height() { return m_config.mapHeight; }

    Biome &Map::BiomeAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, m_config.mapHeight);
        if (index < Map::m_heightMap.size())
            return m_config.biomes[Map::m_biomeMap[index]];
        return m_config.biomes[0];
    }

    float Map::HeightAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, m_config.mapHeight);
        if (index < Map::m_heightMap.size())
            return Map::m_heightMap[index];
        return 0.5f;
    }

    bool Map::WaterAt(Vector2<int> coord)
    {
        uint index = IndexFromCoordinate(coord.x, coord.y, m_config.mapHeight);
        if (index < Map::m_heightMap.size())
            return Map::m_heightMap[index] > m_config.waterLevel;
        return false;
    }
}