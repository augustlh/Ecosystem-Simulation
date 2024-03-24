#include "Map.h"
#include "Renderer.h"

// #include <vector>
#include <cstring>
#include <iostream>
#include <random>

#include <Noise/PerlinNoise.hpp>
#include <yaml/Yaml.hpp>

namespace Ecosim
{
    void SetPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
    {
        uint8_t *pixelAddr = (uint8_t *)surface->pixels + y * surface->pitch + x * sizeof(uint32_t);
        *(uint32_t *)pixelAddr = pixel;
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

                uint32_t waterColor = SDL_MapRGB(surface->format, 51, 77, 102);
                uint32_t landColor = SDL_MapRGB(surface->format, 80, 140, 30);

                uint32_t color = noise > waterHeight ? landColor : waterColor;
                // uint32_t color = waterColor * noise + landColor * (1 - noise);
                // uint32_t color = SDL_MapRGB(surface->format, (uint32_t)(noise * 255.0), (uint32_t)(noise * 255.0), (uint32_t)(noise * 255.0));

                SetPixel(surface, x, y, color);
            }
        }

        SDL_UnlockSurface(surface);
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

        SDL_Surface *surface = Renderer::RequestSDLSurface(mapWidth, mapHeight);
        GenerateMap(surface, waterLevel, smoothness, seed);

        m_texture = Renderer::BakeTexture(surface);
        SDL_FreeSurface(surface);
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