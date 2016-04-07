#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

namespace bb {
    class World;

    class Entity {
    public:
        Entity(World& world) : m_world(world) {
        }
        virtual ~Entity() {
        }
        virtual b2Body* getBody() = 0;
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& window, const double dt) = 0;
    protected:
        World& m_world;
        b2Body* m_body;
    };
}

#endif