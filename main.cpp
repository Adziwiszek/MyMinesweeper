#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace std;

class TileMap : public sf::Drawable, sf::Transformable
{
public:
    TileMap(unsigned int lvlSize)
    {
        vector<int> row(lvlSize, 0);
        tiles = vector<vector<int>>(lvlSize, row);
        numOfBombs = ceil(lvlSize * lvlSize * 0.2);
        this->lvlSize = lvlSize;
    }
    
    bool load(const std::string& tileset, sf::Vector2u tileSize, vector <vector<int>> new_tiles, unsigned int width, unsigned int height, sf::Vector2f startingPos = sf::Vector2f(0.f, 0.f))
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Triangles);
        m_vertices.resize(width * height * 6);

        // populate the vertex array, with two triangles per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = new_tiles[i][j];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the triangles' vertices of the current tile
                sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

                // define the 6 corners of the two triangles
                triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y)             + startingPos;
                triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y)       + startingPos;
                triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y)       + startingPos;
                triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y)       + startingPos;
                triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y)       + startingPos;
                triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y) + startingPos;

                // define the 6 matching texture coordinates
                triangles[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[2].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
                triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

    vector <vector<int>> getTiles()
    {
        return tiles;
    }

private:
    struct Tile
    {
        bool is_bomb = false;
        int bombs_around = 0;
    };
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset; 
    vector<vector<int>> tiles;
    int numOfBombs;
    unsigned int lvlSize;
};



int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 740), "Minesweeper");

    
    const int lvlSize = 10;
    TileMap tile_map(lvlSize);
    tile_map.load("tile.png", sf::Vector2u(64, 64), tile_map.getTiles(), lvlSize, lvlSize, sf::Vector2f(0.f, 100.f));
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("tile.png"))
    {
        return -1;
    }

    while (window.isOpen())
    {
        //handling input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //drawing stuff
        window.clear();
        window.draw(tile_map);
        window.display();
    }

    return 0;
}