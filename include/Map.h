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

namespace Ecosim
{
    class Map
    {
    private:
        SDL_Texture *m_texture;

    public:
        // static Deserialize();
        Map();
        ~Map();
        void Render();
    };
}

#endif