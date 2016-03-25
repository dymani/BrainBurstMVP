#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

namespace bb {
    class World;

    class Enemy : public Entity{
    public:
        Enemy(World& world);
        void update();
        void draw(sf::RenderWindow& window, const double dt);
        sf::Vector2f getCoord();
        void setCoord(float x, float y);
        sf::Vector2f getVelocity();
        void setVelocity(float x, float y);
        sf::FloatRect getHitbox();
    private:
        sf::RectangleShape m_sprite;
    };
}

#endif