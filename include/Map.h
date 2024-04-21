#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>
#include <vector>
#include <string>

#include "Renderer.h"

namespace Ecosim
{
    struct Biome
    {
        std::string name;
        float probability;
        Color color;

        Biome(std::string _name, float _probability, Color _color) : name(_name), probability(_probability), color(_color) {}
    };

    class EnviromentConfig
    {
    public:
        uint seed;
        float smoothness;
        float waterLevel;
        uint mapWidth;
        uint mapHeight;
        uint numBiomes;
        std::vector<Biome> biomes;

        EnviromentConfig() = default;
        EnviromentConfig(std::string &configPath) { EnviromentConfig(configPath.c_str()); }
        EnviromentConfig(const char *configPath);
    };

    class Map
    {
    private:
        static EnviromentConfig m_config;
        static std::vector<uint> m_biomeMap;
        static std::vector<float> m_heightMap;
        static SDL_Surface *m_surface;

    public:
        Map() = delete;

        static void Create(std::string &configPath) { Create(configPath.c_str()); }
        static void Create(const char *configPath);
        static void Cleanup() { SDL_DestroySurface(m_surface); }

        static void Render() { Renderer::Surface(0, 0, m_surface); }

        static uint Width() { return m_config.mapWidth; }
        static uint Height() { return m_config.mapHeight; }

        static Biome &BiomeAt(Vector2<int> coord);
        static float HeightAt(Vector2<int> coord);
        static bool WaterAt(Vector2<int> coord);
    };
}

#endif