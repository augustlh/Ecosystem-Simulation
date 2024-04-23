#include "Map.h"
#include "Renderer.h"
#include "Ecosim.h"
#include "Generation.h"

#include <vector>
#include <iostream>
#include <random>
#include <cmath>

#include <PerlinNoise/PerlinNoise.hpp>
#include <MiniYaml/Yaml.hpp>

namespace Ecosim
{
    void SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
    {
        uint32_t *pixelAddr = (uint32_t *)surface->pixels + y * surface->w + x;
        *pixelAddr = pixel;
    }

    SDL_Surface *CreateMapTexture(uint width, uint height, const std::vector<float> &heightMap, const std::vector<uint> &biomeMap, const std::vector<BiomeType> &biomes, float waterLevel)
    {
        SDL_Surface *surface = Renderer::RequestSurface(width, height);

        Color waterColor = Color(51, 77, 102);

        for (uint x = 0; x < width; ++x)
        {
            for (uint y = 0; y < height; ++y)
            {
                uint biomeIndex = biomeMap[INDEX_FROM_COORD(x, y, height)];

                Color landColor = biomes[biomeIndex].color;
                Color color = heightMap[INDEX_FROM_COORD(x, y, height)] > waterLevel ? landColor : waterColor;

                SetPixel(surface, x, y, color.Format(surface->format));
            }
        }

        return surface;
    }

    std::vector<BiomeType> GenerateBiomeTypes(Yaml::Node &biomesNode)
    {
        std::vector<BiomeType> biomes;

        for (uint i = 0; i < biomesNode.Size(); ++i)
        {
            Yaml::Node &item = biomesNode[i];

            std::string name = item["name"].As<std::string>("<unnamed biome>");
            float probability = item["probability"].As<float>(0.0f);
            Color color = Color::FromHex(item["color"].As<std::string>("#9b57aa"));

            biomes.push_back(BiomeType(name, probability, color));
        }

        if (biomes.size() == 0)
            biomes.emplace_back(BiomeType("Default biome", 1.0f, Color::FromHex("#72ad5d")));

        return biomes;
    }

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
        waterLevel = generation["water-level"].As<float>(0.5f);
        mapWidth = generation["map-width"].As<uint>(800);
        mapHeight = generation["map-height"].As<uint>(800);
        numBiomes = generation["num-biomes"].As<uint>(1);

        biomes = GenerateBiomeTypes(root["biomes"]);

        if (seed == 0)
        {
            std::random_device rd;
            seed = rd();
        }
    }

    EnviromentConfig Map::m_config;
    std::vector<uint> Map::m_biomeMap = {};
    std::vector<float> Map::m_heightMap = {};
    SDL_Surface *Map::m_surface = nullptr;

    void Map::Create(const char *configPath)
    {
        m_config = EnviromentConfig(configPath);

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "Generating map with parameters:\n  - Seed:             %u\n  - Width and height: %ux%u\n  - Smoothness:       %f\n  - Water level:      %f\n  - Number of biomes: %u",
                    m_config.seed, m_config.mapWidth, m_config.mapHeight, m_config.smoothness, m_config.waterLevel, m_config.numBiomes);

        uint width = m_config.mapWidth, height = m_config.mapHeight;

        Generation::Seed(m_config.seed);
        m_heightMap = std::move(Generation::Perlin(width, height, m_config.smoothness, 8));
        m_biomeMap = std::move(Generation::DetailedVoronoi(width, height, m_config.numBiomes, 3));

        std::vector<float> probabilities(m_config.biomes.size(), 0.0f);

        for (int i = 0; i < probabilities.size(); ++i)
            probabilities[i] = m_config.biomes[i].probability;

        Generation::MapVoronoi(m_biomeMap, m_config.numBiomes, probabilities);

        m_surface = CreateMapTexture(width, height, m_heightMap, m_biomeMap, m_config.biomes, m_config.waterLevel);
    }

    BiomeType &Map::BiomeAt(Vector2<int> coord)
    {
        if (coord.x < 0 || coord.x >= Width() ||
            coord.y < 0 || coord.y >= Height())
            return m_config.biomes[0];

        uint index = INDEX_FROM_COORD(coord.x, coord.y, m_config.mapHeight);
        return m_config.biomes[Map::m_biomeMap[index]];
    }

    float Map::HeightAt(Vector2<int> coord)
    {
        if (coord.x < 0 || coord.x >= Width() ||
            coord.y < 0 || coord.y >= Height())
            return 0.5f;

        uint index = INDEX_FROM_COORD(coord.x, coord.y, m_config.mapHeight);
        return Map::m_heightMap[index];
    }

    bool Map::WaterAt(Vector2<int> coord)
    {
        if (coord.x < 0 || coord.x >= Width() ||
            coord.y < 0 || coord.y >= Height())
            return false;

        uint index = INDEX_FROM_COORD(coord.x, coord.y, m_config.mapHeight);
        return Map::m_heightMap[index] < m_config.waterLevel;
    }
}