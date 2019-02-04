#include "noise2d.hpp"

float operator|(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
    return lhs.x*rhs.x+lhs.y*rhs.y;
}

float PerlinNoise::operator()(float x, float y)
{
    sf::Vector2f pos(x, y);
    const int size(generator.getRandSize());
    float noise(0), s(1.0f / scale);
    float sumS(0);

    for(int i = size; i > 0; i /= frequency)
    {
        float fx = std::floor(x / i) * i,
              fy = std::floor(y / i) * i;
        FloatRect2D region(fy + i, fx + i, fy, fx);

        sf::Vector2f g00 = generator.rand2d(region.left, region.bottom),
              g10 = generator.rand2d(static_cast<int>(region.right) % size, region.bottom),
              g01 = generator.rand2d(region.left, static_cast<int>(region.top) % size),
              g11 = generator.rand2d(static_cast<int>(region.right) % size, static_cast<int>(region.top) % size);

        sf::Vector2f d01 = pos - region.tl,         d11 = pos - region.tr,
                     d00 = pos - region.bl,         d10 = pos - region.br;


        for(sf::Vector2f* d : {&d00, &d01, &d10, &d11})
        {
            *d /= static_cast<float>(i);
        }

        /*
        normalize(g00);
        normalize(g01);
        normalize(g10);
        normalize(g11);

        /*normalize(d00);
        normalize(d01);
        normalize(d10);
        normalize(d11);*/

        FloatRect w((g01|d01)/2, (g11|d11)/2, (g00|d00)/2, (g10|d10)/2);

        // https://www.youtube.com/watch?v=MJ3bvCkHJtE&t=949s
        // On divise par 2 car la valeur maximum du produit scalaire est 2:
        // (1,1) . (1,1) = 1x1 + 1x1 = 2

        noise += s * interp->quad(region, w, pos);
        sumS += s;
        s /= scale;
    }

    noise /= sumS;
    noise *= sumS*20;
    return noise;
}
