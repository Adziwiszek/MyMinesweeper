#include <SFML/Graphics.hpp>
#include <cmath>

class TileMap : public sf::Drawable, sf::Transformable
{
private:
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
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 740), "Minesweeper");

    const int level_size = 10;
    int level_info[level_size][level_size] = {};
    int num_of_bombs = ceil(level_size * level_size * 0.2);

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
        window.display();
    }

    return 0;
}