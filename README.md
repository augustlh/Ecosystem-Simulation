## 1. Executing Ecosim

To run Ecosim:

`Ecosim.exe [config.yaml] [fps=24]`

Where `config.yaml` is a path to a valid simulation config file (see section 3.1.) and `fps` is an optional display framerate, with a default value of 24. If no `config.yaml`-path is provided, default values will be set. If `fps` is set to 0, then there will be no fps limit, and the simulation will be rendered every frame.

## 2. Compiling Ecosim

Create a build directory in the root directory:

`mkdir build`\
`cd build`

Setup and compile using CMake:

`cmake ..\`\
`cmake --build .`

## 3. Configuration files

### 3.1. Simulation config specification

The following fields are valid, but not necessary (default values will be set if not provided):

`simulation-name`: The name of the simulation \
`num-agents`: The inital number of agents to simulate \
`total-food`: The total number of food objects \
`enviroment`: A path to a valid enviroment config file (see next section)\
`num-seconds`: The total number of seconds to simulate, or if 0, then run until stopped by user\
`store-data`: Whether or not to save simulation data

### 3.2. Enviroment config specification

The following fields are valid, but not necessary (default values will be set if not provided):

`generation`: A dictionary with the following fields:\
   `seed`: A predefined generation seed, or if 0, a random seed\
   `smoothness`: The smoothness value used for perlin noise sampling\
   Terrain height values are sampled as such: `height = PerlinNoise(x * smoothness, y * smoothness)`\
   `water-level`: Water level in range 0 (no water) to 1 (all water)\
   `map-width`: The width of the map in world coordinates\
   `map-height`: The height of the map in world coordinates\
   `num-biomes`: The number of biomes generated, where each biome have an associated type, specified in `biomes`\
`biomes`: A list of biome types, where each type is a dictionaries with the following fields:\
   `name`: The name of the biome type\
   `color`: The color of the biome type specifed as such: `#rrggbb`\
   `probability`: The probability of a random biome being of this type\
   `visibility`: ... FIXME:\
   `temperature`: ... FIXME:
