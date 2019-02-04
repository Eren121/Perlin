#ifndef NOISE2D_H
#define NOISE2D_H

#include <SFML/System.hpp>
#include <cmath>

template<typename T>
sf::Vector2<T>& normalize(sf::Vector2<T>& v)
{
    if(v.x != 0 && v.y != 0)
    {
        T d = sqrt(v.x*v.x+v.y*v.y);
        v /= d;
    }

    return v;
}

template<typename T>
class Rect {
public:
    Rect(T tl_, T tr_, T bl_, T br_):
        tl(tl_), tr(tr_), bl(bl_), br(br_), topLeft(tl), bottomLeft(bl), topRight(tr), bottomRight(br) {}
    ~Rect() = default;

    T tl, tr, bl, br;
    T &topLeft, &topRight, &bottomLeft, &bottomRight;
};

template<typename T>
class Rect2D : public Rect<sf::Vector2<T>>
{
public:
    typedef sf::Vector2<T> V;
    Rect2D(V tl, V tr, V bl, V br):
        Rect<V>(tl, tr, bl, br), top(tl.y), right(tr.x), left(tl.x), bottom(bl.y) {}
    Rect2D(T top, T right, T bottom, T left):
        Rect2D({left, top}, {right, top}, {left, bottom}, {right, bottom}) {}
    T top, right, left, bottom;
};

typedef Rect<float> FloatRect;
typedef Rect2D<float> FloatRect2D;
typedef Rect2D<int> IntRect2D;

template<typename T>
class Instance
{
public:
    static T instance;
};

template<typename T>
T Instance<T>::instance;

class Interpolation
{
public:

    typedef sf::Vector2f Vec2f;

    float operator()(float x0, float x1, float y0, float y1, float x) const
    {
        float d = interp((x - x0) / (x1 - x0));
        return y0 * (1.0f - d) + y1 * d;
    }

    float quad(const FloatRect2D& rect, const FloatRect& weights, const sf::Vector2f &pos) const
    {
        float wt = (*this)(rect.left, rect.right, weights.tl, weights.tr, pos.x);
        float wb = (*this)(rect.left, rect.right, weights.bl, weights.br, pos.x);
        return (*this)(rect.bottom, rect.top, wb, wt, pos.y);
    }

    virtual float interp(float f) const = 0; // f dans [0;1]
};

class LinearInterpolation : public Interpolation, public Instance<LinearInterpolation>
{
public:
     float interp(float f) const override
    {
        return f;
    }
};

class CosineInterpolation : public Interpolation, public Instance<CosineInterpolation>
{
public:
    float interp(float f) const override
    {
        return 0.5f - std::cos(f * M_PI) / 2.0f;
    }
};

class HermiteInterpolation : public Interpolation, public Instance<HermiteInterpolation>
{
public:
    float interp(float f) const override
    {
        return f*f*f - f*f;
    }
};

class StepInterpolation : public Interpolation, public Instance<StepInterpolation>
{
public:
    float interp(float f) const override
    {
        return f < 0.5f ? 0.0f : 1.0f;
    }
};

class Random2D
{
public:
    Random2D() = default;
    virtual ~Random2D() = default;
    virtual sf::Vector2f rand2d(int x, int y) const = 0; // return dans [-1;1]
    virtual int getRandSize() const = 0;
};

class Noise2D
{
public:
    Noise2D() = default;
    virtual ~Noise2D() = default;
    virtual float operator()(float x, float y) = 0; // noise in [-1;1]
};

class PerlinNoise : public Noise2D
{
public:
    PerlinNoise(const Random2D &g, int frequency = 2, int scale = 2, Interpolation* interp = new CosineInterpolation):
        generator(g), frequency(frequency), scale(scale), interp(interp)
    {}
    ~PerlinNoise() = default;
    float operator()(float x, float y) override;

private:
    const Random2D &generator;
    std::unique_ptr<Interpolation> interp;
    int frequency; // Les puissances à échantilloner (ex: frequency = 2 => 1,2,4,8,...)
    int scale; // Facteur de scale à chaque puissance (ex: scale = 2 => 1,1/2,1/4,1/8...) dans l'ordre décroissant
};

#endif // NOISE2D_H
