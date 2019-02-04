#ifndef WORLD_HPP
#define WORLD_HPP

#include "../chunk.hpp"
#include "../module.hpp"
#include "../noise2d.hpp"

// Monde : une map 2D de 1 million de coordonnées

class World : public Module, private Random2D
{
public:

    World();
    ~World();

    float h(int x, int y) const; // Compute height from (x,y) coordinates

    static sf::Vector2i rand();

    sf::Vector2f rand2d(int i, int j) const override
    {
        //int rx(x[i].x ^ y[j].x);
        //int ry(x[i].y ^ y[j].y);

        int rx(x[(x[i].x * 850000 + j) % x.size()].x),
            ry(x[(y[i].y * 850000 + j) % x.size()].y);

        return {
            static_cast<float>(rx % RAND_MAX) / RAND_MAX * 2.0f - 1.0f,
            static_cast<float>(ry % RAND_MAX) / RAND_MAX * 2.0f - 1.0f
        };
    }

    int getRandSize() const override { return x.size(); }

    void render(), update(), move(int dx, int dy);
    void init();

private:
    void populateXY();

    std::vector<Chunk> chunks; // 9 Chunks loaded : 1 for player, and 8 around
    std::vector<sf::Vector2i> x; // Random values along X axis for X component
    std::vector<sf::Vector2i> y; // Random values along Y axis for X component
    std::vector<std::vector<sf::Color>> colors;
    std::unique_ptr<Noise2D> noise;
    int viewportX, viewportY, zoom;

    float cellSize; // Number of cell in 1 minimal noise cell
};

#endif // WORLD_HPP
