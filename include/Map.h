#ifndef MAP_H
#define MAP_H

#include "Ecosim.h"
#include "Renderer.h"
#include <vector>
#include <string>

namespace Ecosim
{
    /// @brief A structure containing all information relating to a biome-type
    struct BiomeType
    {
        /// @brief The name of the biome-type
        std::string name;

        /// @brief The probability that a random biome will be of this type
        float probability;

        /// @brief The color of the biome-type, when rendered to the screen
        Color color;

        /// @brief Construct a `BiomeType`-object
        /// @param _name The name of the biome-type
        /// @param _probability The probability that a random biome will be of this type
        /// @param _color The color of the biome-type, when rendered to the screen
        BiomeType(std::string _name, float _probability, Color _color) : name(_name), probability(_probability), color(_color) {}
    };

    /// @brief A container for a `enviroments/config.yaml` config file
    class EnviromentConfig
    {
    public:
        uint seed;
        float smoothness;
        float waterLevel;
        uint mapWidth;
        uint mapHeight;
        uint numBiomes;
        std::vector<BiomeType> biomes;

        /// @brief Default constructor
        EnviromentConfig() = default;

        /// @brief Construct a config object given a c++-style filepath
        /// @param configPath A valid filepath
        EnviromentConfig(std::string &configPath) { EnviromentConfig(configPath.c_str()); }

        /// @brief Construct a config object given a null-terminated filepath
        /// @param configPath A valid filepath
        EnviromentConfig(const char *configPath);
    };

    /// @brief A static class representing the entire map
    class Map
    {
    private:
        /// @brief The enviroment config
        static EnviromentConfig m_config;

        /// @brief An array of uints, each value containing the index to a biome in `m_config.biomes`
        static std::vector<uint> m_biomeMap;

        /// @brief An array of floats, each in the interval 0..1 representing the height of the world
        static std::vector<float> m_heightMap;

        /// @brief An actual SDL_Surface for drawing the map to the screen
        static SDL_Surface *m_surface;

    public:
        /// @brief Delete constructor to prevent creating a instance of the Map class
        Map() = delete;

        /// @brief Create a map given a c++-style string to an enviroment config
        /// @param configPath A valid filepath
        static void Create(std::string &configPath) { Create(configPath.c_str()); }

        /// @brief Create a map given a null-terminated string to an enviroment config
        /// @param configPath A valid filepath
        static void Create(const char *configPath);

        /// @brief Deletes the SDL_Surface used for rendering the map
        static void Cleanup() { SDL_DestroySurface(m_surface); }

        /// @brief Render the map to the screen
        static void Render() { Renderer::Surface(0, 0, m_surface); }

        /// @return Return the width of the map
        static uint Width() { return m_config.mapWidth; }

        /// @return Return the height of the map
        static uint Height() { return m_config.mapHeight; }

        /// @brief Get the biome at a certain coordinate
        /// @param coord The coordinate to check
        /// @return A reference to a `BiomeType`
        static BiomeType &BiomeAt(Vector2<int> coord);

        /// @brief Get the height at a certain coordinate
        /// @param coord The coordinate to check
        /// @return The height as a float
        static float HeightAt(Vector2<int> coord);

        /// @brief Check if there is water at a certain coordinate
        /// @param coord The coordinate to check
        /// @return Whether or not there is water
        static bool WaterAt(Vector2<int> coord);
    };
}

#endif