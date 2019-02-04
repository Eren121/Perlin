#ifndef RAND2D_HPP
#define RAND2D_HPP

#include <SFML/Graphics.hpp>
#include "../module.hpp"

class Rand2D : public Module
{
public:
    Rand2D();
    ~Rand2D();

    void update();
    void render();

private:
    void populateXY();

    std::vector<int> x;
    std::vector<int> y;
};

Rand2D::Rand2D() : x(300), y(x.size())
{
    srand(time(0));
    populateXY();
}

Rand2D::~Rand2D() {}

void Rand2D::update()
{
   // populateXY();
}

template<typename T>
sf::Vector2<T> operator/(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs)
{
    return sf::Vector2f(lhs.x / rhs.x, lhs.y / rhs.y);
}

template<typename T>
sf::Vector2<T> operator*(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs)
{
    return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y);
}

void Rand2D::populateXY()
{
    for(int i = 0; i < x.size(); i++)
    {
        x[i] = rand();
    }

    for(int j = 0; j < y.size(); j++)
    {
        y[j] = rand();
    }
}

void Rand2D::render()
{
    static bool fast = true;
    fast = !fast;

    sf::RectangleShape rect(sf::Vector2f(window.getSize()) / sf::Vector2f(x.size(), y.size()));

    for(int i = 0; i < x.size(); i++)
    {
        for(int j = 0; j < y.size(); j++)
        {
            float r = x[i] ^ y[j];
            r = x[(x[i] * 850000 + j) % x.size()];

            //r = 0;
            //if(!fast)
              //  r = rand();

            rect.setFillColor(sf::Color(r, r, r));
            rect.setPosition((sf::Vector2f)rect.getSize() * sf::Vector2f(i, j));
            window.draw(rect);
        }
    }
}

#endif // RAND2D_HPP
