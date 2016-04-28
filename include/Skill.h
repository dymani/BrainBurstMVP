#ifndef SKILL_H
#define SKILL_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;
    class Player;

    class Skill {
    public:
        Skill(World& world, sf::Keyboard::Key key) : m_world(world), m_key(key) {
        }
        virtual ~Skill() {
        }
        virtual void use(Player* player, sf::Vector2f coord) = 0;
        sf::Keyboard::Key getKey() { return m_key; }
        int getHold() { return m_hold; }
        int getTimeout() { return m_timeout; }
        int getSp() { return m_sp; }
    protected:
        World& m_world;
        sf::Keyboard::Key m_key;
        int m_hold = -10;
        int m_timeout = -100;
        int m_sp = 10;
    };
}

#endif