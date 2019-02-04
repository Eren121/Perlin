#include "module.hpp"
#include <SFML/Graphics.hpp>

Module::Module(unsigned int w, unsigned int h, const std::string &title, unsigned int frq) :
    window(sf::VideoMode(w, h), title), frq(frq)
{}

Module::~Module() {}

void Module::run()
{
    sf::Clock clock;

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        if(clock.getElapsedTime().asMilliseconds() > 1000 / frq)
        {
            update();
            clock.restart();
        }

        render();
        window.display();
    }

}
