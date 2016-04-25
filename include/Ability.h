#ifndef ABILITY_H
#define ABILITY_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;
    class Player;

    class Ability {
    public:
        Ability(World& world, sf::Keyboard::Key key) : m_world(world), m_key(key) {
        }
        virtual ~Ability() {
        }
        virtual void use(Player* player, sf::Vector2f coord) = 0;
        virtual sf::Keyboard::Key getKey() = 0;
        virtual int getHold() = 0;
        virtual int getTimeout() = 0;
        virtual int getAp() = 0;
    protected:
        World& m_world;
        sf::Keyboard::Key m_key;
        int m_hold = -50;
        int m_timeout = -200;
        int m_ap = 10;
    };
}

#endif