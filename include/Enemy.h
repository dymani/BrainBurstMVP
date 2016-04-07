#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

namespace bb {
    class World;

    class Enemy : public Entity{
    public:
        Enemy(World& world, float coordX, float coordY);
        void update();
        void draw(sf::RenderWindow& window, const double dt);
        b2Body* getBody();
    private:
        sf::RectangleShape m_sprite;
    };
}

#endif