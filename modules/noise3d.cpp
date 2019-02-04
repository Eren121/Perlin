#include "noise3d.hpp"
#include <ctime>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "../noise2d.hpp"

sf::Vector3f operator/(const sf::Vector3f& lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

float dot(const sf::Vector3f &u, const sf::Vector3f &v)
{
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

float Perlin3D::getNoise(float x, float y, float z) const
{
    sf::Vector3f xyz(x, y, z);
    float n(0);
    float scale(0.5);
    float f(8.0f);
    float octave(2);
    LinearInterpolation mix;

    for(int sample = size; sample > 0; sample /= octave)
    {
        sf::Vector3i pxyz[2][2][2];
        sf::Vector3f gxyz[2][2][2];
        float wxyz[2][2][2];

        for(int i = 0; i < 2; i++) {
            for(int j = 0; j < 2; j++) {
                for(int k = 0; k < 2; k++)
                {
                    int xp = std::floor(x / sample) * sample, yp = std::floor(y / sample) * sample, zp = std::floor(z / sample) * sample;
                    sf::Vector3i& p = pxyz[i][j][k];
                    p.x = xp + (sample*i);
                    p.y = yp + (sample*j);
                    p.z = zp + (sample*k);

                    gxyz[i][j][k] = rnd[p.x % size][p.y % size][p.z % size];
                    wxyz[i][j][k] = dot((xyz - (sf::Vector3f)p) / sample, gxyz[i][j][k]);
                }
            }
        }

        float iz[2][2]; // Interpolation en z

        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                iz[i][j] = mix(pxyz[i][j][0].z, pxyz[i][j][1].z, wxyz[i][j][0], wxyz[i][j][1], z);
            }
        }

        float ixz[2]; // Interpolation en x et en z
        for(int i = 0; i < 2; i++)
        {
            ixz[i] = mix(pxyz[0][i][0].x, pxyz[1][i][0].x, iz[0][i], iz[1][i], x);
        }

        float ixyz = mix(pxyz[0][0][0].y, pxyz[0][1][0].y, ixz[0], ixz[1], y);

        n += f * ixyz / sqrt(3);
        f *= scale;
    }

    n++;
    n /= 2;
    return n;
}


Perlin3D::Perlin3D() : size(128), rnd(size, vector_ndim<2, sf::Vector3f>(size, std::vector<sf::Vector3f>(size)))
{
    srand(time(0));

    for(int i = 0; i < rnd.size(); i++)
    {
        for(int j = 0; j < rnd[0].size(); j++)
        {
            for(int k = 0; k < rnd[0][0].size(); k++)
            {
                rnd[i][j][k].x = rand() / static_cast<float>(RAND_MAX);
                rnd[i][j][k].z = rand() / static_cast<float>(RAND_MAX);
                rnd[i][j][k].y = rand() / static_cast<float>(RAND_MAX);
            }
        }
    }
}

int count = 0;

void Perlin3D::render()
{
    float w = rnd.size();
    float h = rnd[0].size();
    sf::RectangleShape rect({window.getSize().x / w, window.getSize().y / h});

    count++;

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            unsigned char r = getNoise(i, j, count % size) * 255;
            rect.setPosition(i * rect.getSize().x, j * rect.getSize().y);
            rect.setFillColor({r, r, r});
            window.draw(rect);
        }
    }
}

void Perlin3D::update()
{
}
