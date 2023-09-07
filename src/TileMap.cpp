#include "TileMap.h" 
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <queue>

using namespace std;

//Tile class
Tile::Tile()
{
    isCovered = true;
    isBomb = false;
    textureStatus = 0;
    bombsAround = 0;
}

////public

void TileMap::init(unsigned int lvlSize, const string& m_tileset, const string& m_font, float difficulty)
{
    this->lvlSize = lvlSize;
    Tile mT;
    vector<Tile> row(lvlSize, mT);
    tiles = vector<vector<Tile>>(lvlSize, row);

    //creating bombs and placing them on the map
    numOfBombs = ceil(lvlSize * lvlSize * abs(difficulty));
    createBombs(numOfBombs);
    uncoveredBombs = 0;

    if (!tileset.loadFromFile(m_tileset))
    {
        std::cout << "Texture loading error" << std::endl;
    }
    if (!tileFont.loadFromFile(m_font))
    {
        cout << "FONT ERROR" << endl;
    }
}

bool TileMap::load(sf::Vector2u tileSize, vector <vector<Tile>> new_tiles, unsigned int width, unsigned int height, sf::Vector2f startingPos)
{
    // even tho this is never used it needs, for now, to be here 
    // or id doesn't work
    if (!m_tileset.loadFromFile("tile.png"))
        return false;

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(width * height * 6);

    // populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            // get the current tile number
            int tileNumber = new_tiles[i][j].textureStatus;

            // find its position in the tileset texture
            int tu = tileNumber % (tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (tileset.getSize().x / tileSize.x);

            // get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            // define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y) + startingPos;
            triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y) + startingPos;
            triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y) + startingPos;
            triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y) + startingPos;
            triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y) + startingPos;
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

void TileMap::uncoverWholeMap()
{
    for (unsigned int i = 0; i < lvlSize; ++i)
    {
        for (unsigned int j = 0; j < lvlSize; ++j)
        {
           if (tiles[j][i].isBomb)
           {
                tiles[j][i].textureStatus = 2;
           }
           else
           {
                tiles[j][i].textureStatus = 1;
           }
        }
    }
}

void TileMap::uncoverMapAfterClick(sf::Vector2f clickedTile)
{
    //queue with tiles that will be uncovered
    //new tiles that are added are neighbours of some previous tile and have bombsAround = 0
    queue<pair<int, int>> toUncover;
    toUncover.push(make_pair(clickedTile.x, clickedTile.y));
    vector<vector<bool>> checkedTiles = vector<vector<bool>>(lvlSize, vector<bool>(lvlSize, false));

    while (!toUncover.empty())
    {
        int x = toUncover.front().first;
        int y = toUncover.front().second;
        toUncover.pop();

        checkedTiles[x][y] = true;
        tiles[x][y].isCovered = false;
        tiles[x][y].textureStatus = 1;
        //checking right and left
        if (x < lvlSize - 1 && !tiles[x + 1][y].isBomb && !checkedTiles[x + 1][y] && tiles[x][y].bombsAround == 0)
        {
            toUncover.push(make_pair(x + 1, y));
        }
        if (x > 0 && !tiles[x - 1][y].isBomb && !checkedTiles[x - 1][y] && tiles[x][y].bombsAround == 0)
        {
            toUncover.push(make_pair(x - 1, y));
        }
        //checking top and down
        if (y < lvlSize - 1 && !tiles[x][y + 1].isBomb && !checkedTiles[x][y + 1] && tiles[x][y].bombsAround == 0)
        {
            toUncover.push(make_pair(x, y + 1));
        }
        if (y > 0 && !tiles[x][y - 1].isBomb && !checkedTiles[x][y - 1] && tiles[x][y].bombsAround == 0)
        {
            toUncover.push(make_pair(x, y - 1));
        }
    }
}

void TileMap::input(sf::Vector2i mousePos)
{
    auto mouseIsInTile = [](sf::Vector2i mPos, sf::Vector2f tPos, float len) -> bool
    {
            if (mPos.x > tPos.x &&
                mPos.x < tPos.x + len &&
                mPos.y > tPos.y &&
                mPos.y < tPos.y + len)
            {
                return true;
            }
            else return false;
    };

    for (unsigned int i = 0; i < lvlSize; ++i)
    {
        for (unsigned int j = 0; j < lvlSize; ++j)
        {
            if (mouseIsInTile(mousePos, sf::Vector2f(j*64, i*64 + 100), 64))
            {
                if (tiles[j][i].isBomb)
                {
                    tiles[j][i].textureStatus = 2;
                }
                else
                {
                    std::cout << "BOMBS AROUND: " << tiles[j][i].bombsAround << std::endl;
                    tiles[j][i].textureStatus = 1;
                    uncoverMapAfterClick(sf::Vector2f(j, i));
                }
            }
        }
    }
    load(sf::Vector2u(64, 64), tiles, lvlSize, lvlSize, sf::Vector2f(0.f, 100.f));
        
}

vector <vector<Tile>> TileMap::getTiles()
{
    return tiles;
}

void TileMap::drawText(sf::RenderWindow& window)
{
    for (unsigned int i = 0; i < lvlSize; ++i)
    {
        for (unsigned int j = 0; j < lvlSize; ++j)
        {
            if (tiles[i][j].textureStatus != 0 && !tiles[i][j].isBomb && !tiles[i][j].isCovered && tiles[i][j].bombsAround > 0)
            {
                window.draw(tiles[i][j].numOfBombsAroundText);
            }          
        }
    }
}

//private
void TileMap::createBombs(int nBombs)
{
    srand(time(NULL));
    bombsPos.clear();
    vector<int> possiblePlaces;
    for (int i = 0; i < lvlSize * lvlSize; i++)
    {
        possiblePlaces.push_back(i);
    }

    while (nBombs > 0)
    {
        int x = rand() % possiblePlaces.size();
        if (possiblePlaces[x] == 0)
            continue;

        //cout << x << ": " << x % lvlSize << ", " << x / lvlSize << endl;
        bombsPos.push_back(x);
        tiles[x % lvlSize][x / lvlSize].isBomb = true;
        possiblePlaces[x] = 0;
        nBombs--;
    }

    setTilesNumbers();
}

void TileMap::setTilesNumbers()
{
    //calculates tiles around each bomb
    for (unsigned int i = 0; i < bombsPos.size(); i++)
    {
        int x = bombsPos[i] % lvlSize;
        int y = bombsPos[i] / lvlSize;

        //left
        if (x > 0)
        {
            tiles[x - 1][y].bombsAround++;
        }
        //right
        if (x < lvlSize - 1)
        {
            tiles[x + 1][y].bombsAround++;
        }
        //top
        if (y > 0)
        {
            tiles[x][y - 1].bombsAround++;
        }
        //down
        if (y < lvlSize - 1)
        {
            tiles[x][y + 1].bombsAround++;
        }
        //tl
        if (x > 0 && y > 0)
        {
            tiles[x - 1][y - 1].bombsAround++;
        }
        //tr
        if (x < lvlSize - 1 && y > 0)
        {
            tiles[x + 1][y - 1].bombsAround++;
        }
        //bl
        if (x > 0 && y < lvlSize - 1)
        {
            tiles[x - 1][y + 1].bombsAround++;
        }
        //br5
        if (x < lvlSize - 1 && y < lvlSize - 1)
        {
            tiles[x + 1][y + 1].bombsAround++;
        }
    }

    //gives text to each tile
    for (unsigned int i = 0; i < lvlSize; ++i)
    {
        for (unsigned int j = 0; j < lvlSize; ++j)
        {
            tiles[i][j].numOfBombsAroundText.setFont(tileFont);
            tiles[i][j].numOfBombsAroundText.setString(to_string(tiles[i][j].bombsAround));
            tiles[i][j].numOfBombsAroundText.setCharacterSize(36);
            tiles[i][j].numOfBombsAroundText.setFillColor(getTileColor(tiles[i][j].bombsAround));
            tiles[i][j].numOfBombsAroundText.setPosition(sf::Vector2f(i * 64.f + 22, j * 64.f + 100 + 14));
            tiles[i][j].numOfBombsAroundText.setStyle(sf::Text::Bold);
        }
    }

}

sf::Color TileMap::getTileColor(int nBombs)
{
    if (nBombs < 1) return sf::Color::Black;

    if (nBombs == 1) return sf::Color(0, 20, 255);
    if (nBombs == 2) return sf::Color(0, 102, 0);
    if (nBombs == 3) return sf::Color(255, 0, 0);
    if (nBombs == 4) return sf::Color(0, 0, 102);
    if (nBombs == 5) return sf::Color(153, 0, 0);
    return sf::Color::Red;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}

