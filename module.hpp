#ifndef MODULE_HPP
#define MODULE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

class Module
{
public:
    Module(unsigned int w = 1000, unsigned int h = 1000, const std::string &title = "SFML", unsigned int frq = 10);
    virtual ~Module() = 0;
    void run();
protected:
    virtual void update() = 0;
    virtual void render() = 0;

    sf::RenderWindow window;
    int frq;
};

#endif // MODULE_HPP
