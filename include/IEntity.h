#ifndef I_ENTITY_H
#define I_ENTITY_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;

    class IEntity {
    public:
        IEntity(World& world) : m_world(world) {
        }
        virtual ~IEntity() {
        }
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& window, const double dt) = 0;
        virtual sf::Vector2f getCoord() = 0;
        virtual void setCoord(float x, float y) = 0;
    protected:
        World& m_world;
        sf::Vector2f m_coord;
    };
}

#endif