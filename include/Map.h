#ifndef MAP_H
#define MAP_H

/*

Terrain generation
    - Water level: 0..1
    - Perlin noise smoothness: 0..1

Food
    - Number of food pallets: 0..n

Biomes
    - Temperature: -1..1 (cold..hot)
    - Visibility multiplier: 0..2
        Emulate difference between visibility in e.g. a savanna and a dense jungle
    - Food attraction rate: 0..1
        Food picks a random position on the map after it has been eaten or some amount of time has passed
        If a random float in range 0..1 is larger than the enviroments food attraction rate the food spawns, otherwise a new position is picked
    - Colorscheme: r,g,b-color

*/

#include <SDL2/SDL.h>
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

    class Map
    {
    private:
        static uint m_width, m_height;

        static std::vector<uint> m_biomeMap;
        static std::vector<float> m_heightMap;
        static std::vector<Biome> m_biomes;

        static float m_waterLevel;

        static SDL_Texture *m_texture;

    public:
        Map() = delete;

        static void Create(const char *configPath);
        static void Cleanup();

        static void Render();

        static Biome &BiomeAt(Vector2<int> coord);
        static float HeightAt(Vector2<int> coord);
        static bool WaterAt(Vector2<int> coord);
    };
}

#endif