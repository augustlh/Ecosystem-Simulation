#include "Map.h"

Map *Map::instance = nullptr;

Map::Map(float mapRadius) : m_mapRadius(mapRadius)
{
}

Map::~Map()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

Map *Map::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Map(10.0f);
    }
    return instance;
}

float Map::GetMapRadius() const
{
    return m_mapRadius;
}