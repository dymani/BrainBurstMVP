#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

namespace bb {
    class World;

    class Entity {
        friend class Physics;
    public:
        Entity(World& world) : m_world(world) {
        }
        virtual ~Entity() {
        }
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& window, const double dt) = 0;
        virtual sf::Vector2f getCoord() = 0;
        virtual void setCoord(float x, float y) = 0;
        virtual sf::Vector2f getVelocity() = 0;
        virtual void setVelocity(float x, float y) = 0;
        virtual sf::FloatRect getHitbox() = 0;
    protected:
        World& m_world;
        sf::Vector2f m_size, m_coord, m_velocity;
        sf::FloatRect m_hitbox;
        bool m_isOnGround, m_isMovable;
    };
}

#endif