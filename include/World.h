#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Debug.h"

namespace bb {
    class World {
    public:
        World();
        void handleInput(sf::Event event);
        void handleInput();
        bool update();
        void draw(sf::RenderWindow& window, const double dt);
        Debug& getDebug();
    private:
        sf::Font m_font;
        Player* m_player;
        std::map<int, IEntity*> m_entities;
        Debug m_debug;
        bool m_debugShown = false;
    };
}

#endif