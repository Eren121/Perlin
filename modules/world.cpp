#include "world.hpp"
#include <algorithm>
#include <SFML/Graphics.hpp>

const int viewportSize = 128;

template<typename T>
sf::Vector2<T> operator*(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
{
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}

sf::Vector2i World::rand()
{
    return {::rand(), ::rand()};
}

World::World() : x(1e7), y(x.size()), colors(viewportSize, std::vector<sf::Color>(viewportSize)), noise(new PerlinNoise(*this, 100000, 2)), zoom(1), cellSize(.01f)
{
    //viewportX = x.size() / 2;
    //viewportY = y.size() / 2;
    srand(time(0));
    init();
}

void World::init()
{
    std::generate(x.begin(), x.end(), rand);
    std::generate(y.begin(), y.end(), rand);

    move(0, 0);
}

World::~World()
{}

float World::h(int x, int y) const
{
    return 0;
}

void World::move(int dx, int dy)
{
    viewportX += dx;
    viewportY += dy;

    for(int i = 0; i < colors.size(); i++)
    {
        for(int j = 0; j < colors[i].size(); j++)
        {
            int x = viewportX + i*zoom, y = viewportY + j*zoom;

            if(x >= 0 && y >= 0 && x < this->x.size() / cellSize && y < this->y.size() / cellSize)
            {
                float h = (*noise)(x * cellSize, y * cellSize);

                h += 1;
                h /= 2;
                h *= 255;
                h = std::max(std::min(h, 255.0f), 0.0f);
                colors[i][j] = sf::Color(h, h, h);

                //if(h < 0 || h > 256)
                //  colors[i][j] = sf::Color::Red;

                /*if(h < -0.5)
                    colors[i][j] = sf::Color(0, 0, (1.+h) * 300);
                else if(h < 0)
                    colors[i][j] = sf::Color(0, 0, 200 +h * 100);
                else if(h < 0.2)
                    colors[i][j] = sf::Color(50 - h*100, 100 - h * 100, 35);
                else if(h < 0.4)
                    colors[i][j] = sf::Color(160 - (0.9-h)*1600, 82, 45);
                else
                    colors[i][j] = sf::Color(255, 255, 255);*/
            }
            else
            {
                colors[i][j] = sf::Color::Black;
            }
        }
    }
}

void World::render()
{
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(window.getSize()) / static_cast<float>(colors.size()));

    for(int i = 0; i < colors.size(); i++)
    {
        for(int j = 0; j < colors[i].size(); j++)
        {
            rect.setFillColor(colors[i][j]);
            rect.setPosition(rect.getSize() * sf::Vector2f(i, j));
            window.draw(rect);
        }
    }
}

void World::update()
{
    int dx(0), dy(0);
    bool up(false);
    dx += sf::Keyboard::isKeyPressed(sf::Keyboard::D)?1:0;
    dx -= sf::Keyboard::isKeyPressed(sf::Keyboard::Q)?1:0;
    dy += sf::Keyboard::isKeyPressed(sf::Keyboard::Z)?-1:0;
    dy -= sf::Keyboard::isKeyPressed(sf::Keyboard::S)?-1:0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::U))
    {
        init();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        viewportX -= viewportSize * zoom / 2;
        viewportY -= viewportSize * zoom / 2;
        zoom *= 2;
        up = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        if(zoom != 1)
        {
            viewportX += viewportSize * zoom / 4;
            viewportY += viewportSize * zoom / 4;
            zoom /= 2;
            up = true;
        }
    }

    if(dx || dy || up)
    {
        int speed = 50;
        move(dx * speed * zoom, dy * speed * zoom);
    }
}
