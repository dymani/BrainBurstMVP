#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;

    class Gui {
    public:
        Gui(World& world);
        void handleInput();
        void handleInput(sf::Event event);
        void update();
        void draw(const double dt);
    private:
        World& m_world;
        sf::CircleShape m_playerJump, m_playerSprint;
    };
}

#endif