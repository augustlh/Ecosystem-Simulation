#ifndef GENERATION_H
#define GENERATION_H

#include "Ecosim.h"
#include <vector>

#define INDEX_FROM_COORD(x, y, h) static_cast<uint>(x * h + y)

namespace Ecosim::Generation
{
    /// @brief Sets the generation seed
    /// @param seed An unsigned integer
    ECOSIM_API void Seed(uint seed);

    /// @brief Generates voronoi noise in an finite area. The algorithm distributes `numPoints` points in the area defined by `width` and `height`, and calculates the closest point for each pixel in the area.
    /// @param width The width of the area
    /// @param height The height of the area
    /// @param numPoints The number of voronoi-centers
    /// @return An array of unsigned integers with size `width * height`, with each uint containing the index of the closest point
    ECOSIM_API std::vector<uint> Voronoi(uint width, uint height, uint numPoints);

    /// @brief Generates detailed voronoi noise in a finite area, by layering multible layers of voronoi noise on-top of each other
    /// @param width The width of the area
    /// @param height The height of the area
    /// @param numPoints The number of points used to generate the initial layer of voronoi noise
    /// @param detailLayers The number of layers, where the number of points in a given layer `i` is calculated as such: `round(numPoints * 1.6 * (i + 1))`. If `detailLayers = 0` ordinary voronoi noise will be returned
    /// @return An array of unsigned integers with size `width * height`, with each uint in the range `0` to `numPoints - 1`
    ECOSIM_API std::vector<uint> DetailedVoronoi(uint width, uint height, uint numPoints, uint detailLayers);

    /// @brief Generates perlin noise in a finite area defined by `width` and `heigh`
    /// @param width The width of the area
    /// @param height The height of the area
    /// @param smoothness The smoothness, where a smaller value will result in a smaller distance between sampled points
    /// @param octaves The number of perlin-noise layers
    /// @return An array of floats with size `width * height`, where each float is in the range 0..1
    ECOSIM_API std::vector<float> Perlin(uint width, uint height, float smoothness, int octaves);

    /// @brief Maps every index of a voronoi-generated texture given an array of probabilities
    /// @param texture The input texture to remap. This texture will be modified by the function
    /// @param numPoints The number of points used to generate the original texture
    /// @param probabilities An array of probabilities in range 0..1
    ECOSIM_API void MapVoronoi(std::vector<uint> &texture, uint numPoints, const std::vector<float> probabilities);
} // namespace Ecosim::Generation

#endif /* GENERATION_H */