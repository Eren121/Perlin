#ifndef NOISE3D_HPP
#define NOISE3D_HPP

#include <SFML/System.hpp>
#include "../module.hpp"

template<int d, typename T>
struct VectorNDim
{
    typedef typename VectorNDim<d - 1, std::vector<T>>::Type Type;
};

template<typename T>
struct VectorNDim<0, T>
{
    typedef T Type;
};

template<int d, typename T> using vector_ndim = typename VectorNDim<d, T>::Type;

class Perlin3D : public Module
{
public:
    Perlin3D();

    void update();
    void render();

    float getNoise(float x, float y, float z) const;

private:
    int size;
    vector_ndim<3, sf::Vector3f> rnd;
};

#endif // NOISE3D_HPP
