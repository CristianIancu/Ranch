#pragma once
#include <string>

class Map
{
public:

    Map(std::string tID, int ms, int ts);
    ~Map();

    void LoadMap(int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
    static std::string mapData[35][40];
    static bool mapCol[35][40];
    std::string texID;
    int mapScale;
    int tileSize;
    int scaledSize;

};
