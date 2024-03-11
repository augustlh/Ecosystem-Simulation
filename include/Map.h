#ifndef MAP_H
#define MAP_H

class Map
{
public:
    static Map *GetInstance();
    float GetMapRadius() const;
    void RenderMap();

private:
    Map(float mapRadius);
    ~Map();

    static Map *instance;
    float m_mapRadius;
};

#endif